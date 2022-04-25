#include "scoopsoftwarelist.h"
#include <QtCore>
ScoopSoftwareList::ScoopSoftwareList(QMap<QString, QString> env, QString fileName)
    : QSettings(fileName, QSettings::IniFormat), m_env(env)
{
    m_name_list = this->childKeys();
    // qDebug() << m_name_list;
    foreach (QString key, m_name_list)
    {
        QVariantMap status;
        status["name"] = key;
        status["bucket"] = this->value(key).toString();
        status["installed"] = (QDir(m_env["prof"] + "/scoop/apps/" + key).exists());
        // qDebug() << status["name"].toString() <<
        // status["installed"].toBool();
        m_status_map[key] = status;
    }
    // qDebug() << m_status_map;
}
QStringList ScoopSoftwareList::nameList()
{
    return m_name_list;
}
QMap<QString, QVariantMap> ScoopSoftwareList::statusMap()
{
    return m_status_map;
}
