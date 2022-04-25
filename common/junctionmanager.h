#ifndef JUNCTIONMANAGER_H
#define JUNCTIONMANAGER_H

#include <QtCore>

class JunctionManager
{
public:
    JunctionManager();
    bool create(const QString &dest, const QString &src);
    bool remove(const QString &dest);
};

#endif // JUNCTIONMANAGER_H
