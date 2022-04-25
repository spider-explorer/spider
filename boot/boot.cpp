#include "jinstaller.h"
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <singleapplication.h>
#include "jnetwork.h"
#include "junctionmanager.h"
static QString prepareMain(QSplashScreen &splash)
{
    JNetworkManager nm;
    QLocale locale;
    QString spider2Json = nm.getBatchAsText(QUrl("https://gitlab.com/javacommons/spider-release/-/raw/main/spider.json"));
    qDebug().noquote() << spider2Json;
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(spider2Json.trimmed().toUtf8(), &error);
    qDebug() << error.errorString();
    qDebug().noquote() << jsonDoc.toJson();
    QJsonObject object = jsonDoc.object();
    qDebug() << object.keys();
    QString version = object["version"].toString();
    qDebug() << version;
    QJsonArray array = object["url"].toArray();
    QString urlString = array[0].toString();
    qDebug() << urlString;
    splash.showMessage("Spider本体を準備中...", Qt::AlignLeft, Qt::white);
    QString dlPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                     QString("/.spider-explorer/.install/%1.7z").arg(version);
    if (!QFileInfo(dlPath).exists())
    {
        if(true)/**/
        {
            QString parentPath = QFileInfo(dlPath).absolutePath();
            QDir(parentPath).removeRecursively();
        }
        qDebug() << nm.getBatchAsFile(
                     urlString, dlPath,
                     [&splash, &locale](QNetworkReply *reply)
        {
            splash.showMessage(
                QString("Spider本体をダウンロード中...%1")
                .arg(locale.formattedDataSize(reply->bytesAvailable())),
                Qt::AlignLeft, Qt::white);
        });
    }
    QString installDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                         QString("/.spider-explorer/.install/%1").arg(version);
    QString junctionDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                         QString("/.spider-explorer/.install/current").arg(version);
    if (!QFileInfo(installDir).exists())
    {
        qDebug() << extractZip(dlPath, installDir, [&splash, &locale](qint64 extractSizeTotal)
        {
            splash.showMessage(
                QString("Spider本体を展開中...%1").arg(locale.formattedDataSize(extractSizeTotal)),
                Qt::AlignLeft, Qt::white);
        });
        JunctionManager().remove(junctionDir);
        JunctionManager().create(junctionDir, installDir);
    }
    if (!QFileInfo(junctionDir).exists())
    {
        JunctionManager().create(junctionDir, installDir);
    }
    QString mainDll = junctionDir + "/main-x86_64-static.dll";
    return mainDll;
}
int main(int argc, char *argv[])
{
    qDebug() << "main(begin)";
    SingleApplication app(argc, argv, true);
    if (app.isSecondary())
    {
        qDebug() << "[boot] (app.isSecondary())";
        app.sendMessage("dummy");
        return 0;
    }
    QString mainDll;
    // mainDll = QFileInfo(app.applicationDirPath() + "/main-x86_64-static.dll").absoluteFilePath();
    bool force = (app.arguments().size() >= 2 && app.arguments()[1]=="--force");
    mainDll = app.applicationDirPath() + "/main-x86_64-static.dll";
    // QMessageBox::information(nullptr, "確認", mainDll);
    if (force || !QFileInfo(mainDll).exists())
    {
        QSplashScreen splash(QPixmap(":/splash.png"));
        splash.show();
        mainDll = prepareMain(splash);
        splash.finish(nullptr);
    }
    // exit(0);
    QLibrary lib(mainDll);
    typedef void (*prote_entry1)(const char *main_dll_path, int argc, char **argv);
    prote_entry1 entry1 = (prote_entry1)lib.resolve("entry1");
    typedef void (*prote_entry2)();
    prote_entry2 entry2 = (prote_entry2)lib.resolve("entry2");
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     [entry2](quint32 instanceId, QByteArray message)
    {
        qDebug() << "[boot] before calling entry2()";
        entry2();
    });
    QThread *thread =
        QThread::create([entry1, mainDll, argc, argv]
    {
        entry1(mainDll.toStdString().c_str(), argc, argv);
    });
    QObject::connect(thread, &QThread::finished, [&app]()
    {
        app.exit(0);
    });
    thread->start();
    return app.exec();
}
