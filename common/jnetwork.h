#ifndef JNETWORK_H
#define JNETWORK_H

#include <QtCore>
#include <QtNetwork>

using NetworkIdleCallback = std::function<void(QNetworkReply *reply)>;

class JNetworkManager: public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit JNetworkManager(QObject *parent=nullptr);
    //QNetworkReply *lastReply();
    QVariantMap &lastResult();
    // HEAD
protected:
    QNetworkReply *headBatch(const QNetworkRequest &request);
    QNetworkReply *headBatch(const QUrl &url);
public:
    QVariantMap headBatchAsMap(const QNetworkRequest &request);
    QVariantMap headBatchAsMap(const QUrl &url);
    // GET
protected:
    QNetworkReply *getBatch(
            const QNetworkRequest &request,
            NetworkIdleCallback callback = nullptr);
    QNetworkReply *getBatch(
            const QUrl &url,
            NetworkIdleCallback callback = nullptr);
public:
    QVariantMap getBatchAsMap(
            const QNetworkRequest &request,
            NetworkIdleCallback callback = nullptr);
    QVariantMap getBatchAsMap(
            const QUrl &url,
            NetworkIdleCallback callback = nullptr);
    QString getBatchAsText(
            const QNetworkRequest &request,
            NetworkIdleCallback callback = nullptr);
    QString getBatchAsText(
            const QUrl &url,
            NetworkIdleCallback callback = nullptr);
    bool getBatchAsFile(
            const QNetworkRequest &request,
            QString filePath,
            NetworkIdleCallback callback);
    bool getBatchAsFile(
            const QUrl &url,
            QString filePath,
            NetworkIdleCallback callback);
    QVariant getBatchAsJson(
            const QNetworkRequest &request,
            NetworkIdleCallback callback = nullptr);
    QVariant getBatchAsJson(
            const QUrl &url,
            NetworkIdleCallback callback = nullptr);
    // POST
protected:
    QNetworkReply *postBatch(
            const QNetworkRequest &request,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
    QNetworkReply *postBatch(
            const QUrl &url,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
public:
    QVariantMap postBatchAsMap(
            const QNetworkRequest &request,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
    QVariantMap postBatchAsMap(
            const QUrl &url,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
    QVariant postBatchJsonRequest(
            const QNetworkRequest &request,
            const QVariant &data,
            NetworkIdleCallback callback = nullptr);
    QVariant postBatchJsonRequest(
            const QUrl &url,
            const QVariant &data,
            NetworkIdleCallback callback = nullptr);
private:
    QNetworkReply *m_lastReply = nullptr;
    QVariantMap m_lastResult;
};

#endif // JNETWORK_H
