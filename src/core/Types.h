#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QString>
#include <QtGui/QColor>

namespace EtapKitap {

/**
 * @brief Normalized coordinate point on a page (0.0 to 1.0).
 *
 * (0.0, 0.0) is the top-left corner of the page.
 * (1.0, 1.0) is the bottom-right corner of the page.
 * Includes optional pressure value for stylus drawing (0.0 to 1.0).
 */
struct NormalizedPoint {
    qreal x{0.0};
    qreal y{0.0};
    qreal pressure{1.0};

    [[nodiscard]] constexpr bool isValid() const noexcept {
        return x >= 0.0 && x <= 1.0 && y >= 0.0 && y <= 1.0;
    }

    [[nodiscard]] QPointF toPointF() const noexcept {
        return QPointF(x, y);
    }

    [[nodiscard]] static NormalizedPoint fromAbsolute(const QPointF& absPoint, qreal pageWidth, qreal pageHeight, qreal p = 1.0) noexcept {
        if (pageWidth <= 0.0 || pageHeight <= 0.0) {
            return {0.0, 0.0, p};
        }
        return {
            std::clamp(absPoint.x() / pageWidth, 0.0, 1.0),
            std::clamp(absPoint.y() / pageHeight, 0.0, 1.0),
            std::clamp(p, 0.0, 1.0)
        };
    }

    [[nodiscard]] QPointF toAbsolute(qreal pageWidth, qreal pageHeight) const noexcept {
        return QPointF(x * pageWidth, y * pageHeight);
    }

    auto operator<=>(const NormalizedPoint&) const = default;
};

/**
 * @brief Normalized rectangle on a page (0.0 to 1.0).
 */
struct NormalizedRect {
    qreal x{0.0};
    qreal y{0.0};
    qreal width{0.0};
    qreal height{0.0};

    [[nodiscard]] constexpr bool isValid() const noexcept {
        return width > 0.0 && height > 0.0 && x >= 0.0 && y >= 0.0 &&
               (x + width) <= 1.05 && (y + height) <= 1.05; // slight tolerance for rounding
    }

    [[nodiscard]] QRectF toRectF() const noexcept {
        return QRectF(x, y, width, height);
    }

    [[nodiscard]] static NormalizedRect fromAbsolute(const QRectF& absRect, qreal pageWidth, qreal pageHeight) noexcept {
        if (pageWidth <= 0.0 || pageHeight <= 0.0) {
            return {0.0, 0.0, 0.0, 0.0};
        }
        return {
            std::clamp(absRect.x() / pageWidth, 0.0, 1.0),
            std::clamp(absRect.y() / pageHeight, 0.0, 1.0),
            std::clamp(absRect.width() / pageWidth, 0.0, 1.0),
            std::clamp(absRect.height() / pageHeight, 0.0, 1.0)
        };
    }

    [[nodiscard]] QRectF toAbsolute(qreal pageWidth, qreal pageHeight) const noexcept {
        return QRectF(x * pageWidth, y * pageHeight, width * pageWidth, height * pageHeight);
    }

    [[nodiscard]] bool contains(const NormalizedPoint& p) const noexcept {
        return p.x >= x && p.x <= (x + width) && p.y >= y && p.y <= (y + height);
    }

    auto operator<=>(const NormalizedRect&) const = default;
};

/**
 * @brief Active tool modes on the smartboard dock.
 */
enum class ToolType {
    Pointer,         // Navigation & hotspot interaction
    Pen,             // Freehand vector pen
    Highlighter,     // Semitransparent highlighter
    Eraser,          // Stroke or precision eraser
    Line,            // Straight line
    Arrow,           // Directed arrow
    Rectangle,       // Geometric rectangle
    Ellipse,         // Geometric circle/ellipse
    LaserPointer,    // Luminous fading pointer trail
    Spotlight,       // Circular / rectangular spotlight cutout
    Curtain,         // Draggable shade curtain (perde)
    Freeze           // Freeze screen to scratch canvas
};

/**
 * @brief Eraser operational modes.
 */
enum class EraserMode {
    Stroke,          // Deletes the whole stroke when touched
    Precision        // Cuts / splits segments at touch radius
};

/**
 * @brief Page view layouts.
 */
enum class ViewMode {
    SinglePage,      // 1 page centered
    DualPage,        // 2 pages side-by-side (book parity: cover on page 1)
    Continuous       // Continuous vertical scroll
};

/**
 * @brief Zoom fitting modes.
 */
enum class ZoomMode {
    FitWidth,
    FitPage,
    Custom
};

/**
 * @brief Interactive hotspot types supported in .akbook.
 */
enum class InteractionType {
    Video,
    Audio,
    ImageGallery,
    InfoCard,
    TeacherHint,
    Solution,
    Activity,
    Simulation
};

/**
 * @brief Interactive pedagogical activity types.
 */
enum class ActivityType {
    MultipleChoice,
    TrueFalse,
    Matching,
    Ordering,
    FillBlank,
    DragDrop
};

/**
 * @brief Whiteboard background templates.
 */
enum class WhiteboardTemplate {
    BlankWhite,
    BlankBlack,
    Lined,
    Grid,
    Dotted,
    CoordinatePlane
};

/**
 * @brief Curtain (Perde) orientation direction.
 */
enum class CurtainDirection {
    TopToBottom,
    BottomToTop,
    LeftToRight,
    RightToLeft
};

} // namespace EtapKitap
