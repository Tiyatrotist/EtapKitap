#pragma once

#include "core/Types.h"
#include <QtCore/QString>
#include <QtCore/QSizeF>
#include <QtGui/QImage>

namespace EtapKitap {

class IDocument {
public:
    virtual ~IDocument() = default;

    [[nodiscard]] virtual QString id() const = 0;
    [[nodiscard]] virtual QString title() const = 0;
    [[nodiscard]] virtual QString filePath() const = 0;
    [[nodiscard]] virtual int pageCount() const = 0;
    [[nodiscard]] virtual QSizeF pageSize(int pageNumber) const = 0;
    [[nodiscard]] virtual bool isInteractiveBook() const = 0;
    [[nodiscard]] virtual QString pageText(int pageNumber) const = 0;
};

} // namespace EtapKitap
