#pragma once

#include "core/Types.h"
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QUuid>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <memory>

namespace EtapKitap {

class Annotation {
public:
    virtual ~Annotation() = default;

    [[nodiscard]] virtual QString id() const = 0;
    [[nodiscard]] virtual ToolType type() const = 0;
    [[nodiscard]] virtual int pageNumber() const = 0;
    [[nodiscard]] virtual NormalizedRect boundingRect() const = 0;
    [[nodiscard]] virtual bool intersects(const NormalizedPoint& point, qreal tolerance) const = 0;

    virtual void render(QPainter& painter, qreal pageWidth, qreal pageHeight) const = 0;
    [[nodiscard]] virtual QJsonObject toJson() const = 0;
    [[nodiscard]] static std::unique_ptr<Annotation> fromJson(const QJsonObject& json);
};

} // namespace EtapKitap
