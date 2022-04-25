#include "archiver.h"
#include "bashbatch.h"
#include "common.h"
#include "jsonserializer.h"
#include "variantserializer.h"
#include "widget.h"
#include "zip/zip.h" // https://github.com/kuba--/zip
#include <QApplication>
#include <QtNetwork>
#include <quazip5/JlCompress.h>
#include <quazip5/quazip.h>
#include <singleapplication.h>
static QMap<QString, QString> g_env;
static bool downloadFile(QSplashScreen &splash, QString urlString, QString filePath)
{
    QFileInfo info(filePath);
    if (info.exists())
        return true;
    QUrl url(urlString);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
                         "like Gecko) Chrome/100.0.4896.60 Safari/537.36");
    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.get(request);
#if 0x0
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
#else
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
#endif
    QDir(info.absolutePath()).mkpath(".");
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
        reply->deleteLater();
        return true;
    }
    else
    {
        reply->deleteLater();
        return false;
    }
}
static bool extractZip(QSplashScreen &splash, QString zipPath, QString destDir)
{
    QFileInfo info(zipPath);
    qDebug() << "info.exists()" << info.exists();
    if (!info.exists())
        return false;
    QFile zip(zipPath);
#if 0x0
    QStringList result = JlCompress::extractDir(zipPath, destDir);
    qDebug() << result;
#else
    // zip_extract(zipPath.toLocal8Bit().constData(),
    // destDir.toLocal8Bit().constData(), on_extract_entry, nullptr); //
    // https://github.com/kuba--/zip
    extract_archive(zipPath, destDir);
#endif
    return true;
}
static QString prepareProgram(QSplashScreen &splash, QString progName, QString version, QString ext)
{
    splash.showMessage(progName);
    QString dlPath = g_env["prof"] + QString("/.software/%1/%1-%2%3").arg(progName).arg(version).arg(ext);
    QString urlString =
        QString("https://gitlab.com/javacommons/spider/-/raw/main/%1-%2%3").arg(progName).arg(version).arg(ext);
    qDebug() << downloadFile(splash, urlString, dlPath);
    QString installDir = g_env["prof"] + QString("/.software/%1/%2").arg(progName).arg(version);
    if (!QFileInfo(installDir).exists())
    {
        qDebug() << extractZip(splash, dlPath, installDir);
    }
    return installDir;
}
static void prepare(QSplashScreen &splash)
{
    splash.showMessage("prepare(begin)");
    g_env["dir"] = qApp->applicationDirPath();
    g_env["temp"] = g_env["dir"] + "/temp";
    g_env["prof"] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    g_env["docs"] = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    g_env["msys2"] = g_env["prof"] + "/.software/msys2";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    g_env["path"] = np(g_env["dir"] + "/cmd") + ";" + env.value("PATH");
    //
    QString wt_dir = prepareProgram(splash, "WindowsTerminal", "1.12.10393.0", ".7z");
    g_env["wt"] = wt_dir + "/wt.exe";
    //
    QString foldersize_dir = prepareProgram(splash, "FolderSizePortable", "4.9.5.0", ".7z");
    g_env["foldersize"] = foldersize_dir + "/FolderSize64.exe";
    //
    QString sqlitestudio_dir = prepareProgram(splash, "sqlitestudio", "3.3.3", ".7z");
    g_env["sqlitestudio"] = sqlitestudio_dir + "/SQLiteStudio.exe";
    //
    QString rapidee_dir = prepareProgram(splash, "rapidee", "9.2.937", ".7z");
    g_env["rapidee"] = rapidee_dir + "/rapidee.exe";
    //
    QString busybox_dir = prepareProgram(splash, "busybox", "tilde64", ".7z");
    g_env["path"] = np(busybox_dir) + ";" + g_env["path"];
    //
    BashBatch batch(g_env);
    batch.exec("ls -al", g_env["docs"]);
}
int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv, true);
    if (app.isSecondary())
    {
#if 0x0
        QJsonArray array = {"hello", "world", 123};
#else
        QVariantList array = {"hello", "world", 123};
#endif
        // app.sendMessage(JsonSerializer().serialize(array));
        app.sendMessage(VariantSerializer().serialize(array));
        return 0;
    }
    auto font = app.font();
    qDebug() << font.pointSizeF();
    font.setPointSizeF(12.0);
    app.setFont(font);
    QPixmap pixmap(":/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
    // app.processEvents();
    prepare(splash);
    Widget w(g_env);
    QObject::connect(&app, &SingleApplication::receivedMessage,
                     [&w](quint32 instanceId, QByteArray message)
    {
        Q_UNUSED(instanceId);
        w.show();
        w.activateWindow();
        w.raise();
        w.setWindowState(Qt::WindowNoState | Qt::WindowActive);
        // QJsonDocument jdoc =
        // JsonSerializer().deserialize(message);
        // QMessageBox::information(&w, "確認", jdoc.toJson());
        QVariantList list = VariantSerializer().deserialize(message).toList();
        QMessageBox::information(&w, "確認", QString::fromLatin1(message));
#if 0x0
        QJsonArray array = jdoc.array();
        QVariantList list = array.toVariantList();
#else
        // QVariantList list = jdoc.toVariant().toList();
#endif
        foreach (QVariant x, list)
        {
            QMessageBox::information(&w, "確認", x.toString());
        }
    });
    w.show();
    splash.finish(&w);
    return app.exec();
}
