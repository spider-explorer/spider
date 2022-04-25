#include "jnetwork.h"

JNetworkManager::JNetworkManager(QObject *parent)
    : QNetworkAccessManager(parent)
{
}

#if 0x0
QNetworkReply *JNetworkManager::lastReply()
{
    return m_lastReply;
}
#endif

QVariantMap &JNetworkManager::lastResult()
{
    return m_lastResult;
}

QNetworkReply *JNetworkManager::headBatch(const QNetworkRequest &request)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    QNetworkReply *reply = this->head(request2);
    while (!reply->isFinished())
    {
        qApp->processEvents();
    }
    if(m_lastReply != nullptr)
    {
        m_lastReply->deleteLater();
    }
    m_lastReply = reply;
    return reply;
}

QNetworkReply *JNetworkManager::headBatch(const QUrl &url)
{
    return this->headBatch(QNetworkRequest(url));
}

QVariantMap JNetworkManager::headBatchAsMap(const QNetworkRequest &request)
{
    QVariantMap result;
    QNetworkReply *reply = this->headBatch(request);
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
    //qDebug() << "m_lastResult:" << m_lastResult;
    return result;
}

QVariantMap JNetworkManager::headBatchAsMap(const QUrl &url)
{
    return this->headBatchAsMap(QNetworkRequest(url));
}

QNetworkReply *JNetworkManager::getBatch(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    QNetworkReply *reply = this->get(request2);
    while (!reply->isFinished())
    {
        qApp->processEvents();
        if(callback != nullptr)
        {
            callback(reply);
        }
    }
    if(m_lastReply != nullptr)
    {
        m_lastReply->deleteLater();
    }
    m_lastReply = reply;
    return reply;
}

QNetworkReply *JNetworkManager::getBatch(const QUrl &url, NetworkIdleCallback callback)
{
    QNetworkRequest request(url);
    return this->getBatch(request, callback);
}

QVariantMap JNetworkManager::getBatchAsMap(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QVariantMap result;
    QNetworkReply *reply = this->getBatch(request, callback);
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
    //qDebug() << "m_lastResult:" << m_lastResult;
    return result;
}

QVariantMap JNetworkManager::getBatchAsMap(const QUrl &url, NetworkIdleCallback callback)
{
    return this->getBatchAsMap(QNetworkRequest(url), callback);
}

QString JNetworkManager::getBatchAsText(const QNetworkRequest &request, NetworkIdleCallback callback)
{
    QVariantMap result = this->getBatchAsMap(request, callback);
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
    QVariantMap result = this->getBatchAsMap(request, callback);
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
    QVariantMap result = this->getBatchAsMap(request, callback);
    return QJsonDocument::fromJson(result["body"].toByteArray()).toVariant();
}

QVariant JNetworkManager::getBatchAsJson(const QUrl &url, NetworkIdleCallback callback)
{
    return this->getBatchAsJson(QNetworkRequest(url), callback);
}

QNetworkReply *JNetworkManager::postBatch(const QNetworkRequest &request, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    QNetworkRequest request2(request);
    request2.setAttribute(QNetworkRequest::RedirectPolicyAttribute, true);
    request2.setHeader(QNetworkRequest::ContentTypeHeader, contentType);
    QNetworkReply *reply = this->post(request2, data);
    while (!reply->isFinished())
    {
        qApp->processEvents();
        if(callback != nullptr)
        {
            callback(reply);
        }
    }
    if(m_lastReply != nullptr)
    {
        m_lastReply->deleteLater();
    }
    m_lastReply = reply;
    return reply;
}

QNetworkReply *JNetworkManager::postBatch(const QUrl &url, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    return this->postBatch(QNetworkRequest(url), contentType, data, callback);
}

QVariantMap JNetworkManager::postBatchAsMap(const QNetworkRequest &request, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    QVariantMap result;
    QNetworkReply *reply = this->postBatch(request, contentType, data, callback);
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
    //qDebug() << "m_lastResult:" << m_lastResult;
    return result;
}

QVariantMap JNetworkManager::postBatchAsMap(const QUrl &url, const QByteArray &contentType, const QByteArray &data, NetworkIdleCallback callback)
{
    return this->postBatchAsMap(QNetworkRequest(url), contentType, data, callback);
}


QVariant JNetworkManager::postBatchJsonRequest(const QNetworkRequest &request, const QVariant &data, NetworkIdleCallback callback)
{
    QVariantMap result = this->postBatchAsMap(request, "application/json", QJsonDocument::fromVariant(data).toJson(), callback);
    return QJsonDocument::fromJson(result["body"].toByteArray()).toVariant();
}

QVariant JNetworkManager::postBatchJsonRequest(const QUrl &url, const QVariant &data, NetworkIdleCallback callback)
{
    return this->postBatchJsonRequest(QNetworkRequest(url), data, callback);
}
