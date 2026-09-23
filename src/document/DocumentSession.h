#pragma once

#include "core/Types.h"
#include <QtCore/QString>
#include <QtCore/QDateTime>

namespace EtapKitap {

struct DocumentSession {
    QString documentId;
    int currentPage{1};
    qreal zoomLevel{1.0};
    ViewMode viewMode{ViewMode::SinglePage};
    int toolDockSide{1}; // 1 = Right, 0 = Left
    qreal scrollX{0.0};
    qreal scrollY{0.0};
    QDateTime updatedAt{QDateTime::currentDateTimeUtc()};

    [[nodiscard]] bool isValid() const noexcept {
        return !documentId.isEmpty() && currentPage >= 1 && zoomLevel > 0.1;
    }
};

} // namespace EtapKitap
