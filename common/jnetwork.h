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
    QVariantMap &batchResult();
    // HEAD
    QNetworkReply *headRequest(const QNetworkRequest &request, bool batch);
    QVariantMap headBatch(const QNetworkRequest &request);
    QVariantMap headBatch(const QUrl &url);
    // GET
    QNetworkReply *getRequest(
            const QNetworkRequest &request,
            bool batch,
            NetworkIdleCallback callback = nullptr);
    QVariantMap getBatch(
            const QNetworkRequest &request,
            NetworkIdleCallback callback = nullptr);
    QVariantMap getBatch(
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
    QNetworkReply *postRequest(
            const QNetworkRequest &request,
            bool batch,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
    QVariantMap postBatch(
            const QNetworkRequest &request,
            const QByteArray &contentType,
            const QByteArray &data,
            NetworkIdleCallback callback = nullptr);
    QVariantMap postBatch(
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
    //QNetworkReply *m_lastReply = nullptr;
    QVariantMap m_lastResult;
};

#endif // JNETWORK_H
