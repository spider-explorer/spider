#ifndef RECURSIVEFILELISTER_H
#define RECURSIVEFILELISTER_H
#include <QtCore>
using RecursiveFileListerCallback = std::function<bool (QString path)>;
class RecursiveFileLister
{
public:
    RecursiveFileLister();
    QStringList listFiles(QString topDir, RecursiveFileListerCallback callback);
};
#endif // RECURSIVEFILELISTER_H
