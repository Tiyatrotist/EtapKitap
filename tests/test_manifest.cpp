#include <QtTest/QtTest>
#include "book/BookManifest.h"
#include "book/BookPackage.h"
#include "core/ApplicationContext.h"

using namespace EtapKitap;

class TestManifest : public QObject {
    Q_OBJECT

private slots:
    void testValidManifest();
    void testInvalidJson();
    void testMissingRequiredFields();
    void testCoordinatesAndHotspots();
    void testAkbookPackageLoad();
};

void TestManifest::testValidManifest() {
    const QByteArray json = R"({
        "formatVersion": 1,
        "id": "etap-math-9",
        "title": "Matematik 9",
        "language": "tr",
        "document": "document.pdf",
        "chapters": [
            { "title": "1. Mantık", "page": 1 },
            { "title": "2. Kümeler", "page": 15 }
        ],
        "pages": [
            {
                "page": 42,
                "interactions": [
                    {
                        "id": "vid-1",
                        "type": "video",
                        "title": "Çözüm Videosu",
                        "resource": "assets/video/sol.mp4",
                        "rect": { "x": 0.2, "y": 0.3, "width": 0.1, "height": 0.1 }
                    }
                ]
            }
        ]
    })";

    QString err;
    BookManifest manifest = BookManifest::fromJson(json, &err);
    QVERIFY2(err.isEmpty(), qPrintable(err));
    QVERIFY(manifest.isValid());
    QCOMPARE(manifest.id, QStringLiteral("etap-math-9"));
    QCOMPARE(manifest.title, QStringLiteral("Matematik 9"));
    QCOMPARE(manifest.formatVersion, 1);
    QCOMPARE(manifest.chapters.size(), 2);
    QCOMPARE(manifest.chapters[0].title, QStringLiteral("1. Mantık"));
    QCOMPARE(manifest.chapters[0].page, 1);
    QCOMPARE(manifest.pages.size(), 1);
    QCOMPARE(manifest.pages[0].pageNumber, 42);
    QCOMPARE(manifest.pages[0].interactions.size(), 1);

    const auto& hi = manifest.pages[0].interactions[0];
    QCOMPARE(hi.id, QStringLiteral("vid-1"));
    QCOMPARE(hi.type, InteractionType::Video);
    QCOMPARE(hi.resource, QStringLiteral("assets/video/sol.mp4"));
    QVERIFY(qFuzzyCompare(hi.rect.x, 0.2));
    QVERIFY(qFuzzyCompare(hi.rect.y, 0.3));
}

void TestManifest::testInvalidJson() {
    const QByteArray brokenJson = "{\"title\": \"Broken\", ";
    QString err;
    BookManifest manifest = BookManifest::fromJson(brokenJson, &err);
    QVERIFY(!manifest.isValid());
    QVERIFY(!err.isEmpty());
}

void TestManifest::testMissingRequiredFields() {
    const QByteArray missingTitle = R"({ "formatVersion": 1, "id": "test-id" })";
    QString err;
    BookManifest manifest = BookManifest::fromJson(missingTitle, &err);
    QVERIFY(!manifest.isValid());
}

void TestManifest::testCoordinatesAndHotspots() {
    NormalizedRect rect{0.1, 0.2, 0.3, 0.4};
    QVERIFY(rect.isValid());
    QVERIFY(rect.contains(NormalizedPoint{0.2, 0.3}));
    QVERIFY(!rect.contains(NormalizedPoint{0.05, 0.1}));

    QPointF absPt = rect.toAbsolute(1000.0, 2000.0).topLeft();
    QCOMPARE(absPt.x(), 100.0);
    QCOMPARE(absPt.y(), 400.0);
}

void TestManifest::testAkbookPackageLoad() {
    ApplicationContext::instance().initialize();

#ifdef PROJECT_SOURCE_DIR
    const QString demoPackagePath = QStringLiteral(PROJECT_SOURCE_DIR) + QStringLiteral("/demo/matematik9.akbook");
    if (QFile::exists(demoPackagePath)) {
        BookPackage package;
        QString err;
        bool ok = package.open(demoPackagePath, &err);
        QVERIFY2(ok, qPrintable(err));
        QVERIFY(package.isOpen());
        QCOMPARE(package.manifest().id, QStringLiteral("matematik-9-demo"));
        QCOMPARE(package.manifest().title, QStringLiteral("Matematik 9 — Etkileşimli Ders Kitabı"));
        QCOMPARE(package.manifest().chapters.size(), 3);
        QCOMPARE(package.manifest().pages.size(), 5);

        QString q1AssetPath = package.resolveAssetFilePath(QStringLiteral("activities/quizzes/q1.json"));
        QVERIFY(!q1AssetPath.isEmpty());
        QVERIFY(QFile::exists(q1AssetPath));

        QByteArray assetData = package.readAssetData(QStringLiteral("activities/quizzes/q1.json"));
        QVERIFY(!assetData.isEmpty());

        package.close();
        QVERIFY(!package.isOpen());
    }
#endif
    ApplicationContext::instance().shutdown();
}

QTEST_MAIN(TestManifest)
#include "test_manifest.moc"
