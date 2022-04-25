//#include "checksum.h"
#include "utf8LogHandler.h"
#include <QtCore>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(utf8LogHandler);
    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption newOpt(QStringList() << "new", "Project Name.", "name");
    parser.addOption(newOpt);
    if (!parser.parse(qApp->arguments()))
    {
        parser.showHelp(-1);
        return (-1);
    }
    if (parser.isSet(newOpt))
    {
        QString projectName = parser.value(newOpt);
        qDebug() << "--new" << projectName;
        QFile pro1(":/template.pro");
        if(pro1.open(QIODevice::ReadOnly))
        {
            QString proTemplate = QString::fromUtf8(pro1.readAll());
            QFile pro2(QString("%1.pro").arg(projectName));
            if(pro2.open(QIODevice::WriteOnly))
            {
                pro2.write(QString(proTemplate).arg(projectName).toUtf8());
                pro2.close();
                qDebug().noquote() << "Wrote:" << pro2.fileName();
            }
            pro1.close();
        }
        QFile cpp1(":/template.cpp");
        if(cpp1.open(QIODevice::ReadOnly))
        {
            QFile cpp2(QString("%1.cpp").arg(projectName));
            if(cpp2.open(QIODevice::WriteOnly))
            {
                cpp2.write(cpp1.readAll());
                cpp2.close();
                qDebug().noquote() << "Wrote:" << cpp2.fileName();
            }
            else
            {
                qDebug().noquote() << "Could not write:" << cpp2.fileName();
            }
            cpp1.close();
        }
        QFile h1(":/utf8LogHandler.h");
        if(h1.open(QIODevice::ReadOnly))
        {
            QFile h2("utf8LogHandler.h");
            if(h2.open(QIODevice::WriteOnly))
            {
                h2.write(h1.readAll());
                h2.close();
                qDebug().noquote() << "Wrote:" << h2.fileName();
            }
            else
            {
                qDebug().noquote() << "Could not write:" << h2.fileName();
            }
            h1.close();
        }
        return 0;
    }
    parser.showHelp(-1);
    return -1;
    return 0;
}
