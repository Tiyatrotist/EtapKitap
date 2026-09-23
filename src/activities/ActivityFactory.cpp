#include "activities/ActivityFactory.h"
#include "activities/MultipleChoiceActivity.h"
#include "activities/MatchingActivity.h"
#include <QtCore/QJsonDocument>

namespace EtapKitap {

std::unique_ptr<IActivity> ActivityFactory::createFromJson(const QByteArray& jsonData, QString* outError) {
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        if (outError) *outError = QStringLiteral("Activity JSON syntax error: ") + parseErr.errorString();
        return nullptr;
    }
    return createFromJsonObject(doc.object());
}

std::unique_ptr<IActivity> ActivityFactory::createFromJsonObject(const QJsonObject& json) {
    QString type = json.value(QStringLiteral("type")).toString().toLower();
    if (type == QStringLiteral("multiple_choice") || type == QStringLiteral("mcq") || type == QStringLiteral("quiz")) {
        return MultipleChoiceActivity::fromJson(json);
    } else if (type == QStringLiteral("matching")) {
        return MatchingActivity::fromJson(json);
    }
    return nullptr;
}

} // namespace EtapKitap
