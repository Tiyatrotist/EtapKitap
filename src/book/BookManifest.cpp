#include "book/BookManifest.h"
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>

namespace EtapKitap {

QString HotspotInteraction::typeString() const {
    switch (type) {
    case InteractionType::Video: return QStringLiteral("video");
    case InteractionType::Audio: return QStringLiteral("audio");
    case InteractionType::ImageGallery: return QStringLiteral("image");
    case InteractionType::InfoCard: return QStringLiteral("info");
    case InteractionType::TeacherHint: return QStringLiteral("hint");
    case InteractionType::Solution: return QStringLiteral("solution");
    case InteractionType::Activity: return QStringLiteral("activity");
    case InteractionType::Simulation: return QStringLiteral("simulation");
    }
    return QStringLiteral("info");
}

InteractionType HotspotInteraction::stringToType(const QString& str) {
    QString s = str.toLower().trimmed();
    if (s == QStringLiteral("video")) return InteractionType::Video;
    if (s == QStringLiteral("audio") || s == QStringLiteral("sound")) return InteractionType::Audio;
    if (s == QStringLiteral("image") || s == QStringLiteral("gallery")) return InteractionType::ImageGallery;
    if (s == QStringLiteral("hint") || s == QStringLiteral("teacher_hint")) return InteractionType::TeacherHint;
    if (s == QStringLiteral("solution")) return InteractionType::Solution;
    if (s == QStringLiteral("activity") || s == QStringLiteral("quiz")) return InteractionType::Activity;
    if (s == QStringLiteral("simulation") || s == QStringLiteral("sim")) return InteractionType::Simulation;
    return InteractionType::InfoCard;
}

bool BookManifest::isValid() const {
    return formatVersion >= 1 && !id.isEmpty() && !title.isEmpty() && !documentPdf.isEmpty();
}

BookManifest BookManifest::fromJson(const QByteArray& jsonData, QString* outError) {
    BookManifest manifest;
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseErr);
    if (parseErr.error != QJsonParseError::NoError || !doc.isObject()) {
        if (outError) *outError = QStringLiteral("JSON syntax error: ") + parseErr.errorString();
        return manifest;
    }

    QJsonObject root = doc.object();
    manifest.formatVersion = root.value(QStringLiteral("formatVersion")).toInt(1);
    manifest.id = root.value(QStringLiteral("id")).toString();
    manifest.title = root.value(QStringLiteral("title")).toString();
    manifest.language = root.value(QStringLiteral("language")).toString(QStringLiteral("tr"));
    manifest.documentPdf = root.value(QStringLiteral("document")).toString(QStringLiteral("document.pdf"));

    if (manifest.id.isEmpty() || manifest.title.isEmpty()) {
        if (outError) *outError = QStringLiteral("Manifest missing mandatory 'id' or 'title' fields.");
        return manifest;
    }

    // Chapters
    const QJsonArray chaptersArr = root.value(QStringLiteral("chapters")).toArray();
    for (const auto& chVal : chaptersArr) {
        if (chVal.isObject()) {
            QJsonObject chObj = chVal.toObject();
            ChapterInfo ch;
            ch.title = chObj.value(QStringLiteral("title")).toString();
            ch.page = chObj.value(QStringLiteral("page")).toInt(1);
            manifest.chapters.append(ch);
        }
    }

    // Pages & Interactions
    const QJsonArray pagesArr = root.value(QStringLiteral("pages")).toArray();
    for (const auto& pVal : pagesArr) {
        if (!pVal.isObject()) continue;
        QJsonObject pObj = pVal.toObject();
        PageInteractions pi;
        pi.pageNumber = pObj.value(QStringLiteral("page")).toInt(1);

        const QJsonArray interArr = pObj.value(QStringLiteral("interactions")).toArray();
        for (const auto& iVal : interArr) {
            if (!iVal.isObject()) continue;
            QJsonObject iObj = iVal.toObject();
            HotspotInteraction hi;
            hi.id = iObj.value(QStringLiteral("id")).toString();
            hi.type = HotspotInteraction::stringToType(iObj.value(QStringLiteral("type")).toString());
            hi.title = iObj.value(QStringLiteral("title")).toString();
            hi.teaser = iObj.value(QStringLiteral("teaser")).toString();
            hi.resource = iObj.value(QStringLiteral("resource")).toString();
            hi.activityFile = iObj.value(QStringLiteral("activityFile")).toString();
            hi.customData = iObj.value(QStringLiteral("customData")).toObject();

            QJsonObject rectObj = iObj.value(QStringLiteral("rect")).toObject();
            hi.rect.x = rectObj.value(QStringLiteral("x")).toDouble(0.0);
            hi.rect.y = rectObj.value(QStringLiteral("y")).toDouble(0.0);
            hi.rect.width = rectObj.value(QStringLiteral("width")).toDouble(0.06);
            hi.rect.height = rectObj.value(QStringLiteral("height")).toDouble(0.06);

            pi.interactions.append(hi);
        }
        manifest.pages.append(pi);
    }

    return manifest;
}

QByteArray BookManifest::toJson() const {
    QJsonObject root;
    root[QStringLiteral("formatVersion")] = formatVersion;
    root[QStringLiteral("id")] = id;
    root[QStringLiteral("title")] = title;
    root[QStringLiteral("language")] = language;
    root[QStringLiteral("document")] = documentPdf;

    QJsonArray chArr;
    for (const auto& ch : chapters) {
        QJsonObject o;
        o[QStringLiteral("title")] = ch.title;
        o[QStringLiteral("page")] = ch.page;
        chArr.append(o);
    }
    root[QStringLiteral("chapters")] = chArr;

    QJsonArray pArr;
    for (const auto& pi : pages) {
        QJsonObject pObj;
        pObj[QStringLiteral("page")] = pi.pageNumber;

        QJsonArray iArr;
        for (const auto& hi : pi.interactions) {
            QJsonObject iObj;
            iObj[QStringLiteral("id")] = hi.id;
            iObj[QStringLiteral("type")] = hi.typeString();
            iObj[QStringLiteral("title")] = hi.title;
            if (!hi.teaser.isEmpty()) iObj[QStringLiteral("teaser")] = hi.teaser;
            if (!hi.resource.isEmpty()) iObj[QStringLiteral("resource")] = hi.resource;
            if (!hi.activityFile.isEmpty()) iObj[QStringLiteral("activityFile")] = hi.activityFile;
            if (!hi.customData.isEmpty()) iObj[QStringLiteral("customData")] = hi.customData;

            QJsonObject rObj;
            rObj[QStringLiteral("x")] = hi.rect.x;
            rObj[QStringLiteral("y")] = hi.rect.y;
            rObj[QStringLiteral("width")] = hi.rect.width;
            rObj[QStringLiteral("height")] = hi.rect.height;
            iObj[QStringLiteral("rect")] = rObj;

            iArr.append(iObj);
        }
        pObj[QStringLiteral("interactions")] = iArr;
        pArr.append(pObj);
    }
    root[QStringLiteral("pages")] = pArr;

    return QJsonDocument(root).toJson(QJsonDocument::Indented);
}

} // namespace EtapKitap
