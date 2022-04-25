#include "jchecksum.h"
#include "utf8LogHandler.h"
#include <QtCore>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qDebug() << app.arguments();
    if (app.arguments().size() != 2)
        return 1;
    qInstallMessageHandler(utf8LogHandler);
    // qDebug() << "main(begin)";
    QByteArray sha256 = fileChecksum(app.arguments()[1], QCryptographicHash::Sha256);
    // qDebug() << sha256;
    std::cout << sha256.toStdString() << std::endl << std::flush;
    return 0;
}
