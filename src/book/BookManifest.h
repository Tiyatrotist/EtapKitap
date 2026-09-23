#pragma once

#include "core/Types.h"
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QtCore/QJsonObject>

namespace EtapKitap {

struct ChapterInfo {
    QString title;
    int page{1};
};

struct HotspotInteraction {
    QString id;
    InteractionType type{InteractionType::InfoCard};
    NormalizedRect rect;
    QString title;
    QString teaser;
    QString resource;       // Path to video, audio, image etc.
    QString activityFile;   // Path to activity json
    QJsonObject customData; // Additional parameters like hints, solutions

    [[nodiscard]] QString typeString() const;
    static InteractionType stringToType(const QString& str);
};

struct PageInteractions {
    int pageNumber{1};
    QVector<HotspotInteraction> interactions;
};

class BookManifest {
public:
    int formatVersion{1};
    QString id;
    QString title;
    QString language{"tr"};
    QString documentPdf{"document.pdf"};
    QVector<ChapterInfo> chapters;
    QVector<PageInteractions> pages;

    [[nodiscard]] bool isValid() const;
    [[nodiscard]] static BookManifest fromJson(const QByteArray& jsonData, QString* outError = nullptr);
    [[nodiscard]] QByteArray toJson() const;
};

} // namespace EtapKitap
