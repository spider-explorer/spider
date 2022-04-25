#include "jinstaller.h"
#include "jarchiver.h"
//#include "jnetwork.h"
#if 0x0
QString getForText(JNetworkManager &nm, QUrl url, NetworkIdleCallback callback)
{
    qDebug() << "downloadText(1)";
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
                         "like Gecko) Chrome/100.0.4896.60 Safari/537.36");
    QNetworkReply *reply = nm.getBatch(request, callback);
    qDebug() << "downloadText(2)";
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "downloadText(2.1)";
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug() << "downloadText(2.2)";
        qDebug().noquote() << reply->readAll();
        qDebug() << "downloadText(2.3)";
        qDebug() << "downloadText(2.4)";
        reply->deleteLater();
        return "";
    }
    qDebug() << "downloadText(3)";
    QString result = QString::fromUtf8(reply->readAll());
    qDebug() << "downloadText(3.1)";
    //reply->deleteLater();
    qDebug() << "downloadText(4)";
    return result;
}
bool getForFile(JNetworkManager &nm, QUrl url, QString filePath, NetworkIdleCallback callback)
{
    QFileInfo info(filePath);
    if (info.exists())
        return true;
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, "
                         "like Gecko) Chrome/100.0.4896.60 Safari/537.36");
    QNetworkReply *reply = nm.getBatch(request, callback);
    QDir(info.absolutePath()).mkpath(".");
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        return false;
    }
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
#endif
bool extractZip(QString zipPath, QString destDir, ArchiveIdleCallback callback)
{
    QFileInfo info(zipPath);
    qDebug() << "info.exists()" << info.exists();
    if (!info.exists())
        return false;
    QFile zip(zipPath);
    extract_archive(zipPath, destDir, callback);
    return true;
}
