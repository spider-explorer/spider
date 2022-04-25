#ifndef FAVMANAGER_H
#define FAVMANAGER_H
#include <QtCore>
class FavManager
{
public:
    FavManager();
    ~FavManager();
    bool contains(QString path);
    void add_fav(QString path);
    void remove_fav(QString path);
    QStringList favList(QString path);
private:
// QVariantMap m_favMap;
    QSet<QString> m_favSet;
};
#endif // FAVMANAGER_H
