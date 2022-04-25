#ifndef JINSTALLER_H
#define JINSTALLER_H
#include <QtCore>
//#include <QtGui>
//#include <QtNetwork>
//#include <QtWidgets>
//#include "jnetwork.h"
#include "jarchiver.h"
#if 0x0
QString getForText(
        JNetworkManager &nm,
        QUrl url,
        NetworkIdleCallback callback = nullptr);
bool getForFile(
        JNetworkManager &nm,
        QUrl url,
        QString filePath,
        NetworkIdleCallback callback = nullptr);
#endif
bool extractZip(QString zipPath,
                QString destDir,
                ArchiveIdleCallback callback = nullptr);
#endif // JINSTALLER_H
