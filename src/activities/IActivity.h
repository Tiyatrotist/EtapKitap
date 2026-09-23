#pragma once

#include "core/Types.h"
#include <QtCore/QString>
#include <QtCore/QJsonObject>

namespace EtapKitap {

class IActivity {
public:
    virtual ~IActivity() = default;

    [[nodiscard]] virtual QString id() const = 0;
    [[nodiscard]] virtual ActivityType type() const = 0;
    [[nodiscard]] virtual QString title() const = 0;
    [[nodiscard]] virtual QString questionText() const = 0;
    [[nodiscard]] virtual QString teacherHint() const = 0;
    [[nodiscard]] virtual QString solutionExplanation() const = 0;

    virtual void reset() = 0;
    [[nodiscard]] virtual bool isAnswered() const = 0;
    [[nodiscard]] virtual bool validate() = 0;
    [[nodiscard]] virtual qreal score() const = 0; // 0.0 to 1.0

    [[nodiscard]] virtual QJsonObject serializeState() const = 0;
    virtual void restoreState(const QJsonObject& state) = 0;
    [[nodiscard]] virtual QJsonObject toJson() const = 0;
};

} // namespace EtapKitap
