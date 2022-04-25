#ifndef DATETIMEMANAGER_H
#define DATETIMEMANAGER_H
#include <QtCore>
class DateTimeManager
{
public:
    DateTimeManager();
    QString toString(const QDateTime &dt);
    QDateTime fromString(const QString &s);
};
#endif // DATETIMEMANAGER_H
