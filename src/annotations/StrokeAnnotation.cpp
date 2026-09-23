#include "annotations/StrokeAnnotation.h"
#include <QtCore/QJsonArray>
#include <QtGui/QPainterPath>
#include <cmath>

namespace EtapKitap {

static qreal distanceToSegment(const QPointF& p, const QPointF& a, const QPointF& b) {
    const qreal l2 = (b.x() - a.x()) * (b.x() - a.x()) + (b.y() - a.y()) * (b.y() - a.y());
    if (l2 == 0.0) {
        return std::hypot(p.x() - a.x(), p.y() - a.y());
    }
    const qreal t = std::clamp(((p.x() - a.x()) * (b.x() - a.x()) + (p.y() - a.y()) * (b.y() - a.y())) / l2, 0.0, 1.0);
    const QPointF projection = a + t * (b - a);
    return std::hypot(p.x() - projection.x(), p.y() - projection.y());
}

StrokeAnnotation::StrokeAnnotation(int pageNumber, ToolType type)
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_pageNumber(pageNumber)
    , m_type(type)
{
    if (m_type == ToolType::Highlighter) {
        m_color = QColor(255, 235, 59, 120); // translucent yellow
        m_opacity = 0.5;
        m_width = 0.020; // thicker for highlighting
    }
}

QString StrokeAnnotation::id() const { return m_id; }
ToolType StrokeAnnotation::type() const { return m_type; }
int StrokeAnnotation::pageNumber() const { return m_pageNumber; }
NormalizedRect StrokeAnnotation::boundingRect() const { return m_boundingRect; }

void StrokeAnnotation::addPoint(const NormalizedPoint& pt) {
    m_points.append(pt);
    updateBoundingRect();
}

const QVector<NormalizedPoint>& StrokeAnnotation::points() const {
    return m_points;
}

void StrokeAnnotation::setPoints(const QVector<NormalizedPoint>& points) {
    m_points = points;
    updateBoundingRect();
}

QColor StrokeAnnotation::color() const { return m_color; }
void StrokeAnnotation::setColor(const QColor& c) { m_color = c; }

qreal StrokeAnnotation::opacity() const { return m_opacity; }
void StrokeAnnotation::setOpacity(qreal o) { m_opacity = std::clamp(o, 0.0, 1.0); }

qreal StrokeAnnotation::width() const { return m_width; }
void StrokeAnnotation::setWidth(qreal w) { m_width = std::max(0.001, w); }

void StrokeAnnotation::setId(const QString& id) { m_id = id; }

void StrokeAnnotation::updateBoundingRect() {
    if (m_points.isEmpty()) {
        m_boundingRect = NormalizedRect{};
        return;
    }

    qreal minX = m_points.first().x;
    qreal maxX = minX;
    qreal minY = m_points.first().y;
    qreal maxY = minY;

    for (const auto& pt : m_points) {
        minX = std::min(minX, pt.x);
        maxX = std::max(maxX, pt.x);
        minY = std::min(minY, pt.y);
        maxY = std::max(maxY, pt.y);
    }

    const qreal pad = m_width / 2.0;
    m_boundingRect.x = std::max(0.0, minX - pad);
    m_boundingRect.y = std::max(0.0, minY - pad);
    m_boundingRect.width = std::min(1.0 - m_boundingRect.x, (maxX - minX) + 2.0 * pad);
    m_boundingRect.height = std::min(1.0 - m_boundingRect.y, (maxY - minY) + 2.0 * pad);
}

bool StrokeAnnotation::intersects(const NormalizedPoint& point, qreal tolerance) const {
    if (m_points.size() < 2) {
        if (m_points.size() == 1) {
            return std::hypot(point.x - m_points[0].x, point.y - m_points[0].y) <= (m_width + tolerance);
        }
        return false;
    }

    const qreal threshold = (m_width / 2.0) + tolerance;
    const QPointF p(point.x, point.y);

    for (int i = 0; i < m_points.size() - 1; ++i) {
        QPointF a(m_points[i].x, m_points[i].y);
        QPointF b(m_points[i + 1].x, m_points[i + 1].y);
        if (distanceToSegment(p, a, b) <= threshold) {
            return true;
        }
    }
    return false;
}

void StrokeAnnotation::render(QPainter& painter, qreal pageWidth, qreal pageHeight) const {
    if (m_points.size() < 2) {
        return;
    }

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    QColor drawColor = m_color;
    if (m_type == ToolType::Highlighter) {
        painter.setCompositionMode(QPainter::CompositionMode_Multiply);
        drawColor.setAlphaF(m_opacity * 0.7);
    } else {
        drawColor.setAlphaF(m_opacity);
    }

    const qreal pixelWidth = m_width * pageWidth;
    QPen pen(drawColor, pixelWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    QPainterPath path;
    path.moveTo(m_points[0].x * pageWidth, m_points[0].y * pageHeight);
    for (int i = 1; i < m_points.size(); ++i) {
        path.lineTo(m_points[i].x * pageWidth, m_points[i].y * pageHeight);
    }
    painter.drawPath(path);

    painter.restore();
}

QJsonObject StrokeAnnotation::toJson() const {
    QJsonObject obj;
    obj[QStringLiteral("id")] = m_id;
    obj[QStringLiteral("type")] = (m_type == ToolType::Highlighter) ? QStringLiteral("highlighter") : QStringLiteral("stroke");
    obj[QStringLiteral("page")] = m_pageNumber;
    obj[QStringLiteral("color")] = m_color.name(QColor::HexArgb);
    obj[QStringLiteral("opacity")] = m_opacity;
    obj[QStringLiteral("width")] = m_width;

    QJsonArray ptsArr;
    for (const auto& pt : m_points) {
        QJsonArray ptItem;
        ptItem.append(pt.x);
        ptItem.append(pt.y);
        ptItem.append(pt.pressure);
        ptsArr.append(ptItem);
    }
    obj[QStringLiteral("points")] = ptsArr;
    return obj;
}

std::unique_ptr<StrokeAnnotation> StrokeAnnotation::fromJson(const QJsonObject& json) {
    int page = json.value(QStringLiteral("page")).toInt(1);
    QString typeStr = json.value(QStringLiteral("type")).toString();
    ToolType tool = (typeStr == QStringLiteral("highlighter")) ? ToolType::Highlighter : ToolType::Pen;

    auto stroke = std::make_unique<StrokeAnnotation>(page, tool);
    stroke->setId(json.value(QStringLiteral("id")).toString(QUuid::createUuid().toString(QUuid::WithoutBraces)));
    stroke->setColor(QColor(json.value(QStringLiteral("color")).toString(QStringLiteral("#E53935"))));
    stroke->setOpacity(json.value(QStringLiteral("opacity")).toDouble(1.0));
    stroke->setWidth(json.value(QStringLiteral("width")).toDouble(0.004));

    QVector<NormalizedPoint> points;
    const QJsonArray ptsArr = json.value(QStringLiteral("points")).toArray();
    for (const auto& val : ptsArr) {
        if (val.isArray()) {
            QJsonArray a = val.toArray();
            NormalizedPoint pt;
            pt.x = a.size() > 0 ? a.at(0).toDouble(0.0) : 0.0;
            pt.y = a.size() > 1 ? a.at(1).toDouble(0.0) : 0.0;
            pt.pressure = a.size() > 2 ? a.at(2).toDouble(1.0) : 1.0;
            points.append(pt);
        }
    }
    stroke->setPoints(points);
    return stroke;
}

std::unique_ptr<Annotation> Annotation::fromJson(const QJsonObject& json) {
    QString type = json.value(QStringLiteral("type")).toString();
    if (type == QStringLiteral("stroke") || type == QStringLiteral("highlighter")) {
        return StrokeAnnotation::fromJson(json);
    }
    return nullptr;
}

} // namespace EtapKitap
