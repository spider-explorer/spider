#include "utf8LogHandler.h"
#include <QtCore>
#include <QtNetwork>
#include <iostream>
static QMap<QString, QString> g_env;
QJsonDocument ignoredList(QString cwd)
{
    QProcess proc;
    if (!cwd.isEmpty())
    {
        cwd = QFileInfo(cwd).absoluteFilePath();
        proc.setWorkingDirectory(cwd);
    }
    proc.setProgram("cmd.exe");
    proc.setArguments(QStringList() << "/c"
                      << "git.exe"
                      << "status"
                      << "--ignored"
                      << "--short");
    proc.start();
    proc.waitForFinished();
    QString output = QString::fromUtf8(proc.readAllStandardOutput());
    // qDebug() << output;
    // qDebug().noquote() << output;
    QStringList list = (output.trimmed()).split("\n");
    list = list.filter(QRegExp("^!!", Qt::CaseSensitive));
    // qDebug() << list;
    QStringList result;
    foreach (QString x, list)
    {
        x = x.replace(QRegExp("^!! "), "");
        if (x.startsWith("../"))
            continue;
        if (!cwd.isEmpty())
        {
            x = cwd + "/" + x;
        }
        x = x.replace(QRegExp("/$"), "");
        result.append(x);
    }
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(result);
    return jsonDoc;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(utf8LogHandler);
    g_env["dir"] = qApp->applicationDirPath();
    g_env["temp"] = g_env["dir"] + "/temp";
    g_env["prof"] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    g_env["docs"] = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    g_env["msys2"] = g_env["prof"] + "/.software/msys2";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    g_env["path"] = env.value("PATH");
    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption actionOpt(QStringList() << "action", "Action. ignored", "action");
    parser.addOption(actionOpt);
    if (!parser.parse(qApp->arguments()))
    {
        parser.showHelp(-1);
        return (-1);
    }
    if (!parser.isSet(actionOpt))
    {
        parser.showHelp(-1);
        return -1;
    }
    QString action = parser.value(actionOpt);
    if (action == "ignored")
    {
        // qDebug() << actionOpt.description();
        QJsonDocument result =
            ignoredList(parser.positionalArguments().size() >= 1 ? parser.positionalArguments()[0] : "");
        std::cout << result.toJson().trimmed().toStdString() << std::endl << std::flush;
        return 0;
    }
    parser.showHelp(-1);
    return -1;
    return 0;
}
