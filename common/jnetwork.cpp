#include "jnetwork.h"

JNetworkManager::JNetworkManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

QVariantMap &JNetworkManager::batchResult()
{
    return m_lastResult;
}

QNetworkReply *JNetworkManager::headRequest(const QNetworkRequest &request, bool batch)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    QNetworkReply *reply = this->head(request2);
    if(batch)
    {
        while (!reply->isFinished())
        {
            qApp->processEvents();
        }
    }
    return reply;
}

QVariantMap JNetworkManager::headBatch(const QNetworkRequest &request)
{
    QVariantMap result;
    QNetworkReply *reply = this->headRequest(request, true);
    QByteArray body;
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        body = "";
    }
    else
    {
        body = reply->readAll();
    }
    QList<QByteArray> headerNameList = reply->rawHeaderList();
    foreach(QByteArray headerName, headerNameList)
    {
        result[QString::fromLatin1(headerName)] = reply->rawHeader(headerName);
    }
    result["httpStatus"] = httpStatus;
    result["body"] = body;
    m_lastResult = result;
    reply->deleteLater();
    return result;
}

QVariantMap JNetworkManager::headBatch(const QUrl &url)
{
    return this->headBatch(QNetworkRequest(url));
}

QNetworkReply *JNetworkManager::getRequest(const QNetworkRequest &request, bool batch, NetworkIdleCallback callback)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    QNetworkReply *reply = this->get(request2);
    if(batch)
    {
        while (!reply->isFinished())
        {
            qApp->processEvents();
            if(callback != nullptr)
            {
                callback(reply);
            }
        }
    }
    return reply;
}

QVariantMap JNetworkManager::getBatch(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QVariantMap result;
    QNetworkReply *reply = this->getRequest(request, true, callback);
    QByteArray body;
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        body = "";
    }
    else
    {
        body = reply->readAll();
    }
    QList<QByteArray> headerNameList = reply->rawHeaderList();
    foreach(QByteArray headerName, headerNameList)
    {
        result[QString::fromLatin1(headerName)] = reply->rawHeader(headerName);
    }
    result["httpStatus"] = httpStatus;
    result["body"] = body;
    m_lastResult = result;
    reply->deleteLater();
    return result;
}

QVariantMap JNetworkManager::getBatch(const QUrl &url, NetworkIdleCallback callback)
{
    return this->getBatch(QNetworkRequest(url), callback);
}

QString JNetworkManager::getBatchAsText(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QVariantMap result = this->getBatch(request, callback);
    return QString::fromUtf8(result["body"].toByteArray());
}

QString JNetworkManager::getBatchAsText(const QUrl &url, NetworkIdleCallback callback)
{
    return this->getBatchAsText(QNetworkRequest(url), callback);
}

bool JNetworkManager::getBatchAsFile(const QNetworkRequest &request, QString filePath, NetworkIdleCallback callback)
{
    QFileInfo info(filePath);
    if (info.exists())
        return true;
    QDir(info.absolutePath()).mkpath(".");
    QVariantMap result = this->getBatch(request, callback);
    int httpStatus = result["httpStatus"].toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        return false;
    }
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(result["body"].toByteArray());
        file.close();
        return true;
    }
    else
    {
        return false;
    }
}

bool JNetworkManager::getBatchAsFile(const QUrl &url, QString filePath, NetworkIdleCallback callback)
{
    return this->getBatchAsFile(QNetworkRequest(url), filePath, callback);

}

QVariant JNetworkManager::getBatchAsJson(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QVariantMap result = this->getBatch(request, callback);
    return QJsonDocument::fromJson(result["body"].toByteArray()).toVariant();
}

QVariant JNetworkManager::getBatchAsJson(const QUrl &url, NetworkIdleCallback callback)
{
    return this->getBatchAsJson(QNetworkRequest(url), callback);
}

QNetworkReply *JNetworkManager::postRequest(const QNetworkRequest &request, bool batch, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    //request2.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    QNetworkReply *reply = this->post(request2, data);
    if(batch)
    {
        while (!reply->isFinished())
        {
            qApp->processEvents();
            if(callback != nullptr)
            {
                callback(reply);
            }
        }
    }
    return reply;
}

QVariantMap JNetworkManager::postBatch(const QNetworkRequest &request, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    QVariantMap result;
    QNetworkReply *reply = this->postRequest(request, true, contentType, data, callback);
    QByteArray body;
    int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (httpStatus < 200 || httpStatus >= 300)
    {
        qDebug().noquote() << reply->readAll();
        body = "";
    }
    else
    {
        body = reply->readAll();
    }
    QList<QByteArray> headerNameList = reply->rawHeaderList();
    foreach(QByteArray headerName, headerNameList)
    {
        result[QString::fromLatin1(headerName)] = reply->rawHeader(headerName);
    }
    result["httpStatus"] = httpStatus;
    result["body"] = body;
    m_lastResult = result;
    reply->deleteLater();
    return result;
}

QVariantMap JNetworkManager::postBatch(const QUrl &url, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    return this->postBatch(QNetworkRequest(url), contentType, data, callback);
}


QVariant JNetworkManager::postBatchJsonRequest(const QNetworkRequest &request, const QVariant &data, NetworkIdleCallback callback)
{
    QVariantMap result = this->postBatch(request, "application/json", QJsonDocument::fromVariant(data).toJson(), callback);
    return QJsonDocument::fromJson(result["body"].toByteArray()).toVariant();
}

QVariant JNetworkManager::postBatchJsonRequest(const QUrl &url, const QVariant &data, NetworkIdleCallback callback)
{
    return this->postBatchJsonRequest(QNetworkRequest(url), data, callback);
}
