#pragma once

#include "activities/IActivity.h"
#include <memory>

namespace EtapKitap {

class ActivityFactory {
public:
    static std::unique_ptr<IActivity> createFromJson(const QByteArray& jsonData, QString* outError = nullptr);
    static std::unique_ptr<IActivity> createFromJsonObject(const QJsonObject& json);
};

} // namespace EtapKitap
