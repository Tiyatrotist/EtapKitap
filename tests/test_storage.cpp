#include <QtTest/QtTest>
#include <QtCore/QTemporaryDir>
#include <QtSql/QSqlQuery>
#include "storage/DatabaseManager.h"
#include "storage/SessionRepository.h"
#include "storage/AnnotationRepository.h"
#include "annotations/StrokeAnnotation.h"

using namespace EtapKitap;

class TestStorage : public QObject {
    Q_OBJECT

private slots:
    void testDatabaseAndMigrations();
    void testSessionAndRecents();
    void testAnnotationPersistence();
};

void TestStorage::testDatabaseAndMigrations() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString dbPath = tempDir.filePath(QStringLiteral("test_library.db"));
    DatabaseManager dbManager(dbPath);
    QVERIFY(dbManager.open());
    QVERIFY(dbManager.isOpen());

    // Verify tables exist
    QSqlQuery q(dbManager.database());
    QVERIFY(q.exec(QStringLiteral("SELECT count(*) FROM schema_version;")));
    QVERIFY(q.next());
    QCOMPARE(q.value(0).toInt(), 1);

    dbManager.close();
    QVERIFY(!dbManager.isOpen());
}

void TestStorage::testSessionAndRecents() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString dbPath = tempDir.filePath(QStringLiteral("test_session.db"));
    DatabaseManager dbManager(dbPath);
    QVERIFY(dbManager.open());

    SessionRepository repo(&dbManager);

    // Record document
    QVERIFY(repo.recordDocument(QStringLiteral("doc1"), QStringLiteral("/path/to/doc.pdf"), QStringLiteral("Fizik 9"), QStringLiteral("pdf"), 180));

    // Save session
    DocumentSession session;
    session.documentId = QStringLiteral("doc1");
    session.currentPage = 42;
    session.zoomLevel = 1.5;
    session.viewMode = ViewMode::DualPage;
    QVERIFY(repo.saveSession(session));

    // Restore session
    auto loaded = repo.loadSession(QStringLiteral("doc1"));
    QVERIFY(loaded.has_value());
    QCOMPARE(loaded->currentPage, 42);
    QVERIFY(qFuzzyCompare(loaded->zoomLevel, 1.5));
    QCOMPARE(loaded->viewMode, ViewMode::DualPage);

    // Recent documents
    auto recents = repo.getRecentDocuments(5);
    QCOMPARE(recents.size(), 1);
    QCOMPARE(recents[0].title, QStringLiteral("Fizik 9"));
    QCOMPARE(recents[0].lastPage, 42);

    dbManager.close();
}

void TestStorage::testAnnotationPersistence() {
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString dbPath = tempDir.filePath(QStringLiteral("test_ann.db"));
    DatabaseManager dbManager(dbPath);
    QVERIFY(dbManager.open());

    AnnotationRepository annRepo(&dbManager);
    SessionRepository sessionRepo(&dbManager);
    sessionRepo.recordDocument(QStringLiteral("docA"), QStringLiteral("/a.pdf"), QStringLiteral("Doc A"), QStringLiteral("pdf"), 10);

    auto stroke = std::make_shared<StrokeAnnotation>(5, ToolType::Pen);
    stroke->addPoint(NormalizedPoint{0.1, 0.2});
    stroke->addPoint(NormalizedPoint{0.3, 0.4});

    QVector<std::shared_ptr<Annotation>> list = { stroke };
    QVERIFY(annRepo.savePageAnnotations(QStringLiteral("docA"), 5, list));

    auto loaded = annRepo.loadPageAnnotations(QStringLiteral("docA"), 5);
    QCOMPARE(loaded.size(), 1);
    QCOMPARE(loaded[0]->pageNumber(), 5);
    QCOMPARE(loaded[0]->id(), stroke->id());

    dbManager.close();
}

QTEST_MAIN(TestStorage)
#include "test_storage.moc"
