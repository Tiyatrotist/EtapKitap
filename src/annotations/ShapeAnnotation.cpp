#include "annotations/ShapeAnnotation.h"
#include <QtGui/QPolygonF>
#include <cmath>

namespace EtapKitap {

ShapeAnnotation::ShapeAnnotation(int pageNumber, ToolType shapeType)
    : m_id(QUuid::createUuid().toString(QUuid::WithoutBraces))
    , m_pageNumber(pageNumber)
    , m_shapeType(shapeType)
{
}

QString ShapeAnnotation::id() const { return m_id; }
ToolType ShapeAnnotation::type() const { return m_shapeType; }
int ShapeAnnotation::pageNumber() const { return m_pageNumber; }

void ShapeAnnotation::setStartPoint(const NormalizedPoint& pt) { m_startPoint = pt; }
void ShapeAnnotation::setEndPoint(const NormalizedPoint& pt) { m_endPoint = pt; }
NormalizedPoint ShapeAnnotation::startPoint() const { return m_startPoint; }
NormalizedPoint ShapeAnnotation::endPoint() const { return m_endPoint; }

QColor ShapeAnnotation::color() const { return m_color; }
void ShapeAnnotation::setColor(const QColor& c) { m_color = c; }

qreal ShapeAnnotation::width() const { return m_width; }
void ShapeAnnotation::setWidth(qreal w) { m_width = std::max(0.001, w); }

bool ShapeAnnotation::isFilled() const { return m_filled; }
void ShapeAnnotation::setFilled(bool filled) { m_filled = filled; }
QColor ShapeAnnotation::fillColor() const { return m_fillColor; }
void ShapeAnnotation::setFillColor(const QColor& c) { m_fillColor = c; }
void ShapeAnnotation::setId(const QString& id) { m_id = id; }

NormalizedRect ShapeAnnotation::boundingRect() const {
    qreal minX = std::min(m_startPoint.x, m_endPoint.x);
    qreal maxX = std::max(m_startPoint.x, m_endPoint.x);
    qreal minY = std::min(m_startPoint.y, m_endPoint.y);
    qreal maxY = std::max(m_startPoint.y, m_endPoint.y);

    const qreal pad = m_width / 2.0;
    return NormalizedRect{
        std::max(0.0, minX - pad),
        std::max(0.0, minY - pad),
        std::min(1.0, (maxX - minX) + 2.0 * pad),
        std::min(1.0, (maxY - minY) + 2.0 * pad)
    };
}

bool ShapeAnnotation::intersects(const NormalizedPoint& point, qreal tolerance) const {
    const auto rect = boundingRect();
    const qreal tol = tolerance + (m_width / 2.0);
    return (point.x >= rect.x - tol && point.x <= rect.x + rect.width + tol &&
            point.y >= rect.y - tol && point.y <= rect.y + rect.height + tol);
}

void ShapeAnnotation::render(QPainter& painter, qreal pageWidth, qreal pageHeight) const {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, true);

    const qreal pxWidth = m_width * pageWidth;
    QPen pen(m_color, pxWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    painter.setPen(pen);

    if (m_filled && m_fillColor.isValid()) {
        painter.setBrush(QBrush(m_fillColor));
    } else {
        painter.setBrush(Qt::NoBrush);
    }

    const QPointF p1(m_startPoint.x * pageWidth, m_startPoint.y * pageHeight);
    const QPointF p2(m_endPoint.x * pageWidth, m_endPoint.y * pageHeight);

    switch (m_shapeType) {
    case ToolType::Line:
        painter.drawLine(p1, p2);
        break;

    case ToolType::Arrow: {
        painter.drawLine(p1, p2);
        // Arrow head
        const qreal dx = p2.x() - p1.x();
        const qreal dy = p2.y() - p1.y();
        const qreal angle = std::atan2(dy, dx);
        const qreal arrowLen = std::max(12.0, pxWidth * 4.0);
        const qreal arrowAngle = 0.45; // ~26 degrees

        QPointF a1 = p2 - QPointF(arrowLen * std::cos(angle - arrowAngle), arrowLen * std::sin(angle - arrowAngle));
        QPointF a2 = p2 - QPointF(arrowLen * std::cos(angle + arrowAngle), arrowLen * std::sin(angle + arrowAngle));

        QPolygonF head;
        head << p2 << a1 << a2;
        painter.setBrush(QBrush(m_color));
        painter.drawPolygon(head);
        break;
    }

    case ToolType::Rectangle: {
        QRectF r(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
                 std::abs(p2.x() - p1.x()), std::abs(p2.y() - p1.y()));
        painter.drawRoundedRect(r, 4.0, 4.0);
        break;
    }

    case ToolType::Ellipse: {
        QRectF r(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
                 std::abs(p2.x() - p1.x()), std::abs(p2.y() - p1.y()));
        painter.drawEllipse(r);
        break;
    }

    default:
        break;
    }

    painter.restore();
}

QJsonObject ShapeAnnotation::toJson() const {
    QJsonObject obj;
    obj[QStringLiteral("id")] = m_id;
    obj[QStringLiteral("page")] = m_pageNumber;
    obj[QStringLiteral("color")] = m_color.name(QColor::HexArgb);
    obj[QStringLiteral("width")] = m_width;
    obj[QStringLiteral("filled")] = m_filled;
    obj[QStringLiteral("fillColor")] = m_fillColor.name(QColor::HexArgb);

    QString sType = QStringLiteral("rect");
    if (m_shapeType == ToolType::Line) sType = QStringLiteral("line");
    else if (m_shapeType == ToolType::Arrow) sType = QStringLiteral("arrow");
    else if (m_shapeType == ToolType::Ellipse) sType = QStringLiteral("ellipse");
    obj[QStringLiteral("type")] = sType;

    QJsonObject startObj;
    startObj[QStringLiteral("x")] = m_startPoint.x;
    startObj[QStringLiteral("y")] = m_startPoint.y;
    obj[QStringLiteral("start")] = startObj;

    QJsonObject endObj;
    endObj[QStringLiteral("x")] = m_endPoint.x;
    endObj[QStringLiteral("y")] = m_endPoint.y;
    obj[QStringLiteral("end")] = endObj;

    return obj;
}

std::unique_ptr<ShapeAnnotation> ShapeAnnotation::fromJson(const QJsonObject& json) {
    int page = json.value(QStringLiteral("page")).toInt(1);
    QString sType = json.value(QStringLiteral("type")).toString();
    ToolType tool = ToolType::Rectangle;
    if (sType == QStringLiteral("line")) tool = ToolType::Line;
    else if (sType == QStringLiteral("arrow")) tool = ToolType::Arrow;
    else if (sType == QStringLiteral("ellipse")) tool = ToolType::Ellipse;

    auto shape = std::make_unique<ShapeAnnotation>(page, tool);
    shape->setId(json.value(QStringLiteral("id")).toString(QUuid::createUuid().toString(QUuid::WithoutBraces)));
    shape->setColor(QColor(json.value(QStringLiteral("color")).toString(QStringLiteral("#E53935"))));
    shape->setWidth(json.value(QStringLiteral("width")).toDouble(0.004));
    shape->setFilled(json.value(QStringLiteral("filled")).toBool(false));
    shape->setFillColor(QColor(json.value(QStringLiteral("fillColor")).toString(QStringLiteral("#00000000"))));

    QJsonObject startObj = json.value(QStringLiteral("start")).toObject();
    shape->setStartPoint(NormalizedPoint{startObj.value(QStringLiteral("x")).toDouble(0.0),
                                         startObj.value(QStringLiteral("y")).toDouble(0.0)});

    QJsonObject endObj = json.value(QStringLiteral("end")).toObject();
    shape->setEndPoint(NormalizedPoint{endObj.value(QStringLiteral("x")).toDouble(0.0),
                                       endObj.value(QStringLiteral("y")).toDouble(0.0)});
    return shape;
}

} // namespace EtapKitap
