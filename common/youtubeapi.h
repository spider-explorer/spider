#ifndef YOUTUBEAPI_H
#define YOUTUBEAPI_H
#include <QtCore>
class YoutubeAPI
{
public:
    YoutubeAPI();
    QVariantList searchVideo(const QString &text, bool highDefinition = false);
private:
    QString m_key;
    QString key();
};
#endif // YOUTUBEAPI_H
