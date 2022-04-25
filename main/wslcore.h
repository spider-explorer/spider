#ifndef WSLCORE_H
#define WSLCORE_H
#include <QtCore>
class WslCore
{
public:
    WslCore();
    void test();
    QStringList distroList();
    QMap<QString, QString> guiAppMap(QString distro);
    QMap<QString, QString> linkMap(QString linkPath);
    QStringList parseArguments(QString argument);
};
#endif // WSLCORE_H
