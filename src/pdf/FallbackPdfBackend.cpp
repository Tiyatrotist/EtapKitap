#include "pdf/FallbackPdfBackend.h"
#include "core/Logger.h"
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QFont>
#include <QtGui/QPen>
#include <QtGui/QBrush>

namespace EtapKitap {

FallbackPdfBackend::FallbackPdfBackend() = default;

bool FallbackPdfBackend::load(const QString& filePath) {
    m_filePath = filePath;
    QFileInfo fi(filePath);
    m_title = fi.baseName();
    if (m_title.isEmpty()) {
        m_title = QStringLiteral("Ders Kitabı");
    }
    m_pageCount = 20; // Default sample pages
    m_isLoaded = true;
    qCInfo(logPdf) << "FallbackPdfBackend loaded:" << filePath << "with" << m_pageCount << "pages.";
    return true;
}

void FallbackPdfBackend::close() {
    m_isLoaded = false;
    m_filePath.clear();
}

bool FallbackPdfBackend::isLoaded() const {
    return m_isLoaded;
}

int FallbackPdfBackend::pageCount() const {
    return m_isLoaded ? m_pageCount : 0;
}

QSizeF FallbackPdfBackend::pageSize(int /*pageNumber*/) const {
    return m_standardPageSize;
}

QImage FallbackPdfBackend::renderPage(int pageNumber, const QSize& targetPixelSize) {
    if (!m_isLoaded || targetPixelSize.isEmpty()) {
        return QImage();
    }

    QImage img(targetPixelSize, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::white);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);

    const qreal w = targetPixelSize.width();
    const qreal h = targetPixelSize.height();
    const qreal scale = w / m_standardPageSize.width();

    // Top Header Banner
    const qreal headerHeight = 60.0 * scale;
    QRectF headerRect(0, 0, w, headerHeight);
    QLinearGradient grad(0, 0, w, 0);
    grad.setColorAt(0.0, QColor(24, 76, 120));
    grad.setColorAt(1.0, QColor(41, 128, 185));
    p.fillRect(headerRect, grad);

    // Header Text
    p.setPen(Qt::white);
    QFont titleFont(QStringLiteral("Segoe UI"), static_cast<int>(16 * scale), QFont::Bold);
    p.setFont(titleFont);
    p.drawText(QRectF(20 * scale, 0, w - 40 * scale, headerHeight),
               Qt::AlignVCenter | Qt::AlignLeft,
               m_title + QStringLiteral(" — Ünite 3: Denklem ve Eşitsizlikler"));

    // Page Border Frame
    p.setPen(QPen(QColor(220, 224, 230), 1.5 * scale));
    p.drawRect(QRectF(15 * scale, 15 * scale, w - 30 * scale, h - 30 * scale));

    // Content Text Sections
    p.setPen(QColor(40, 44, 52));
    QFont bodyFont(QStringLiteral("Segoe UI"), static_cast<int>(12 * scale), QFont::Normal);
    p.setFont(bodyFont);

    const qreal marginX = 40.0 * scale;
    qreal curY = headerHeight + 35.0 * scale;

    // Section Title
    QFont secFont(QStringLiteral("Segoe UI"), static_cast<int>(15 * scale), QFont::DemiBold);
    p.setFont(secFont);
    p.setPen(QColor(180, 40, 40));
    p.drawText(QRectF(marginX, curY, w - 2 * marginX, 30 * scale),
               Qt::AlignVCenter | Qt::AlignLeft,
               QStringLiteral("3.2 İkinci Dereceden Bir Bilinmeyenli Denklemler"));
    curY += 40.0 * scale;

    // Body Paragraph
    p.setFont(bodyFont);
    p.setPen(QColor(60, 64, 70));
    QString body = QStringLiteral(
        "Bu bölümde ikinci dereceden bir bilinmeyenli denklemlerin köklerinin bulunması, "
        "diskriminant (Δ) kavramı ve kökler arasındaki bağıntılar incelenecektir.\n\n"
        "Tanım: a, b, c ∈ R ve a ≠ 0 olmak üzere ax² + bx + c = 0 biçimindeki ifadelere "
        "ikinci dereceden bir bilinmeyenli denklem denir.\n"
        "Diskriminant: Δ = b² - 4ac ile hesaplanır."
    );
    p.drawText(QRectF(marginX, curY, w - 2 * marginX, 120 * scale),
               Qt::TextWordWrap, body);
    curY += 130.0 * scale;

    // Boxed Example / Problem
    QRectF boxRect(marginX, curY, w - 2 * marginX, 140 * scale);
    p.setBrush(QColor(245, 248, 252));
    p.setPen(QPen(QColor(70, 130, 180), 2.0 * scale));
    p.drawRoundedRect(boxRect, 8 * scale, 8 * scale);

    p.setPen(QColor(30, 60, 100));
    QFont boldFont(QStringLiteral("Segoe UI"), static_cast<int>(13 * scale), QFont::Bold);
    p.setFont(boldFont);
    p.drawText(QRectF(boxRect.x() + 15 * scale, boxRect.y() + 10 * scale, boxRect.width() - 30 * scale, 25 * scale),
               QStringLiteral("Örnek Soru %1:").arg(pageNumber));

    p.setFont(bodyFont);
    p.setPen(QColor(50, 50, 50));
    QString example = QStringLiteral(
        "x² - 5x + 6 = 0 denkleminin çözüm kümesini bulunuz.\n"
        "A) {1, 6}      B) {2, 3}      C) {-2, -3}      D) {-1, 6}\n\n"
        "[Sayfa üzerindeki etkileşim simgelerine dokunarak video çözümünü izleyebilir veya soru etkinliğini çözebilirsiniz.]"
    );
    p.drawText(QRectF(boxRect.x() + 15 * scale, boxRect.y() + 40 * scale, boxRect.width() - 30 * scale, 90 * scale),
               Qt::TextWordWrap, example);
    curY += 160.0 * scale;

    // Mathematical diagram / geometry representation
    QRectF diagRect(marginX + 20 * scale, curY, 200 * scale, 120 * scale);
    p.setBrush(QColor(250, 250, 250));
    p.setPen(QPen(QColor(180, 180, 180), 1.0 * scale));
    p.drawRect(diagRect);

    // Coordinate grid inside diagram
    p.setPen(QPen(QColor(230, 230, 230), 1.0 * scale));
    for (int i = 1; i < 6; ++i) {
        p.drawLine(diagRect.left(), diagRect.top() + i * 20 * scale, diagRect.right(), diagRect.top() + i * 20 * scale);
        p.drawLine(diagRect.left() + i * 33 * scale, diagRect.top(), diagRect.left() + i * 33 * scale, diagRect.bottom());
    }
    // Axes
    p.setPen(QPen(QColor(100, 100, 100), 1.5 * scale));
    p.drawLine(diagRect.left(), diagRect.center().y(), diagRect.right(), diagRect.center().y());
    p.drawLine(diagRect.left() + 50 * scale, diagRect.top(), diagRect.left() + 50 * scale, diagRect.bottom());

    // Parabola curve y = (x-2)(x-3)
    p.setPen(QPen(QColor(220, 53, 69), 2.5 * scale));
    QPainterPath path;
    const qreal ox = diagRect.left() + 50 * scale;
    const qreal oy = diagRect.center().y();
    path.moveTo(ox - 30 * scale, oy - 40 * scale);
    path.quadTo(ox + 40 * scale, oy + 45 * scale, ox + 110 * scale, oy - 40 * scale);
    p.drawPath(path);

    // Footer
    const qreal footerY = h - 35.0 * scale;
    p.setPen(QColor(120, 125, 130));
    p.setFont(QFont(QStringLiteral("Segoe UI"), static_cast<int>(10 * scale)));
    p.drawText(QRectF(0, footerY, w, 25 * scale),
               Qt::AlignCenter,
               QStringLiteral("— Sayfa %1 —").arg(pageNumber));

    p.end();
    return img;
}

QString FallbackPdfBackend::pageText(int pageNumber) const {
    return QStringLiteral("İkinci dereceden bir bilinmeyenli denklemler diskriminant kökler formülü soru %1").arg(pageNumber);
}

QString FallbackPdfBackend::title() const {
    return m_title;
}

void FallbackPdfBackend::setPageCount(int count) {
    m_pageCount = qMax(1, count);
}

void FallbackPdfBackend::setTitle(const QString& title) {
    m_title = title;
}

} // namespace EtapKitap
