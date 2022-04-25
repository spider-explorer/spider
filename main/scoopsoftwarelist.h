#ifndef SCOOPSOFTWARELIST_H
#define SCOOPSOFTWARELIST_H
#include <QtCore>
class ScoopSoftwareList : public QSettings
{
public:
    ScoopSoftwareList(QMap<QString, QString> env, QString fileName);
    QStringList nameList();
    QMap<QString, QVariantMap> statusMap();
private:
    QMap<QString, QString> m_env;
    QStringList m_name_list;
    QMap<QString, QVariantMap> m_status_map;
};
#endif // SCOOPSOFTWARELIST_H
