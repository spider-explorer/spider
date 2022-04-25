#include "favmanager.h"
#include "datetimemanager.h"
#include "spidercore.h"
const QString FAVORITE_KEY = "favorite";
FavManager::FavManager()
{
    QStringList favList = g_settings().value(FAVORITE_KEY).toStringList();
    m_favSet = QSet<QString>(favList.constBegin(), favList.constEnd());
}
FavManager::~FavManager()
{
    QStringList favList = m_favSet.values();
    favList.sort();
    g_settings().setValue(FAVORITE_KEY, favList);
}
bool FavManager::contains(QString path)
{
    return m_favSet.contains(path);
}
void FavManager::add_fav(QString path)
{
    qDebug() << "FavManager::add_fav():" << path;
    // qDebug() << "FavManager::add_fav():" << m_favSet;
    // QString now = DateTimeManager().toString(QDateTime::currentDateTime());
    // qDebug() << "FavManager::add_fav():" << now;
    // m_favMap.insert(path, now);
    // m_favMap[path] = now;
    m_favSet.insert(path);
    qDebug() << m_favSet;
}
void FavManager::remove_fav(QString path)
{
    m_favSet.remove(path);
}
QStringList FavManager::favList(QString path)
{
    QStringList pathList;
    QStringList keys = m_favSet.values();
    keys.sort();
    foreach (auto key, keys)
    {
        if (key.startsWith(path + "/"))
        {
            if (QFileInfo(key).exists())
            {
                pathList.append(key);
            }
        }
    }
    return pathList;
}
