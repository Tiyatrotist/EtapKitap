#include "app/AppController.h"
#include "core/ApplicationContext.h"
#include "core/Logger.h"
#include "ui/DrawingCanvasItem.h"
#include "ui/WhiteboardCanvasItem.h"
#include "ui/PageImageProvider.h"

#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtCore/QCommandLineParser>
#include <QtCore/QFileInfo>

using namespace EtapKitap;

int main(int argc, char* argv[]) {
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("Tiyatrotist"));
    app.setOrganizationDomain(QStringLiteral("tiyatrotist.github.io"));
    app.setApplicationName(QStringLiteral("EtapKitap"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));

    initializeLogging();
    ApplicationContext::instance().initialize();

    qmlRegisterType<DrawingCanvasItem>("EtapKitap", 1, 0, "DrawingCanvasItem");
    qmlRegisterType<WhiteboardCanvasItem>("EtapKitap", 1, 0, "WhiteboardCanvasItem");

    QQmlApplicationEngine engine;

    auto appCtrl = std::make_unique<AppController>();

    // Add Image provider for smooth progressive page rendering
    engine.addImageProvider(QStringLiteral("page"), new PageImageProvider(appCtrl->docCtrl()));

    // Expose appCtrl to QML context
    engine.rootContext()->setContextProperty(QStringLiteral("appCtrl"), appCtrl.get());

    // Command line arguments for file opening
    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Pardus ETAP Etkilesimli Dijital Ders Kitabi"));
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument(QStringLiteral("file"), QStringLiteral("Acilacak PDF veya .akbook dosyasi"));
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
        const QString filePath = QFileInfo(args.first()).absoluteFilePath();
        QMetaObject::invokeMethod(appCtrl.get(), [ctrl = appCtrl.get(), filePath]() {
            ctrl->openDocument(filePath);
        }, Qt::QueuedConnection);
    }

    const QUrl url(QStringLiteral("qrc:/EtapKitap/ui/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    int ret = app.exec();
    ApplicationContext::instance().shutdown();
    return ret;
}
