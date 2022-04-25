#include <QtCore>
#include <QtGui>
#include <QtQml>

#include "utf8LogHandler.h"
#include "app-data.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    qInstallMessageHandler(utf8LogHandler);

    if(app.arguments().size() != 2)
    {
        qDebug() << "QML path not specified. exiting.";
        return -1;
    }

    qmlRegisterType<ApplicationData>("app", 1, 0, "ApplicationData");
    qmlRegisterType<ApplicationFactory>("app", 1, 0, "ApplicationFactory");

    QQmlApplicationEngine engine;
    QFileInfo info(app.arguments()[1]);
    //QString appDir = app.applicationDirPath();
    const QUrl url = QUrl::fromLocalFile(info.absoluteFilePath());

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
