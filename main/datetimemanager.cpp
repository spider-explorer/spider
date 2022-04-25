#include "datetimemanager.h"
DateTimeManager::DateTimeManager()
{
}
QString DateTimeManager::toString(const QDateTime &dt)
{
    return dt.toString("yyyy/MM/dd-hh:mm:ss.zzz");
}
QDateTime DateTimeManager::fromString(const QString &s)
{
    return QDateTime::fromString(s, "yyyy/MM/dd-hh:mm:ss.zzz");
}
