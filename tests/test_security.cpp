#include <QtTest/QtTest>
#include "book/BookSecurityValidator.h"

using namespace EtapKitap;

class TestSecurity : public QObject {
    Q_OBJECT

private slots:
    void testPathTraversalRejection();
    void testAllowedExtensions();
    void testCompressionRatioAndSize();
};

void TestSecurity::testPathTraversalRejection() {
    // Dangerous paths
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("../secret.txt")));
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("/etc/shadow")));
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("\\Windows\\System32")));
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("C:/boot.ini")));
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("assets/../../evil.sh")));
    QVERIFY(!BookSecurityValidator::isSafeRelativePath(QStringLiteral("")));

    // Safe relative paths
    QVERIFY(BookSecurityValidator::isSafeRelativePath(QStringLiteral("manifest.json")));
    QVERIFY(BookSecurityValidator::isSafeRelativePath(QStringLiteral("document.pdf")));
    QVERIFY(BookSecurityValidator::isSafeRelativePath(QStringLiteral("assets/images/cover.png")));
    QVERIFY(BookSecurityValidator::isSafeRelativePath(QStringLiteral("activities/quizzes/q1.json")));
}

void TestSecurity::testAllowedExtensions() {
    // Dangerous extensions
    QVERIFY(!BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("malware.exe")));
    QVERIFY(!BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("script.sh")));
    QVERIFY(!BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("payload.bat")));
    QVERIFY(!BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("plugin.so")));
    QVERIFY(!BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("driver.dll")));

    // Safe extensions
    QVERIFY(BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("document.pdf")));
    QVERIFY(BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("image.png")));
    QVERIFY(BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("video.mp4")));
    QVERIFY(BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("audio.mp3")));
    QVERIFY(BookSecurityValidator::isAllowedAssetExtension(QStringLiteral("vector.svg")));
}

void TestSecurity::testCompressionRatioAndSize() {
    // Normal file
    QVERIFY(BookSecurityValidator::isSafeFileSize(1024 * 1024, 500 * 1024));

    // Exceeds max single file size
    QVERIFY(!BookSecurityValidator::isSafeFileSize(600 * 1024 * 1024LL, 1024));

    // Zip bomb ratio (1000:1 with large size)
    QVERIFY(!BookSecurityValidator::isSafeFileSize(50 * 1024 * 1024LL, 10 * 1024));
}

QTEST_MAIN(TestSecurity)
#include "test_security.moc"
