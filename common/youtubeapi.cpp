#include "youtubeapi.h"
#include "jnetwork.h"

YoutubeAPI::YoutubeAPI()
{
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_key = env.value("YOUTUBE_API_KEY");
}

QVariantList YoutubeAPI::searchVideo(const QString &text, bool highDefinition)
{
    QUrl url("https://www.googleapis.com/youtube/v3/search");
    QUrlQuery query;
    query.addQueryItem("key", this->key());
    query.addQueryItem("q", text);
    query.addQueryItem("type", "video");
    query.addQueryItem("videoEmbeddable", "true");
    //query.addQueryItem("order", "viewCount");
    query.addQueryItem("part", "snippet, id");
    query.addQueryItem("maxResults", "50");
    if(highDefinition)
    {
        query.addQueryItem("videoDefinition", "high");
    }
    url.setQuery(query.query());
    qDebug() << url;
    JNetworkManager nm;
    QString result = nm.getBatchAsText(url);
    //QString result = QString::fromUtf8(reply1->readAll());
    //qDebug().noquote() << result;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(result.toUtf8());
    QVariantMap map = jsonDoc.toVariant().toMap();
    QVariantList list = map["items"].toList();
    qDebug() << list.size();
    QVariantList vList;
    //ui->listWidget->clear();
    for(int i=0; i<list.size(); i++)
    {
        QVariantMap item = list[i].toMap();
        QString kind = item["id"].toMap()["kind"].toString();
        if(kind != "youtube#video") continue;
        QString videoId = item["id"].toMap()["videoId"].toString();
        qDebug() << videoId;
        QVariantMap snippet = item["snippet"].toMap();
        QString title = snippet["title"].toString();
        //QString urlString = snippet["thumbnails"].toMap()["default"].toMap()["url"].toString();
        QString urlString = snippet["thumbnails"].toMap()["high"].toMap()["url"].toString();
        QVariantMap map;
        map["videoId"] = videoId;
        map["title"] = title;
        map["thumbnail"] = urlString;
        vList.append(map);
    }
    return vList;
}

QString YoutubeAPI::key()
{
    return m_key;
}
