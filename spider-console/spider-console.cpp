#include "utf8LogHandler.h"
#include <QtCore>
#include <iostream>
static inline QString np(QString x)
{
    return x.replace("/", "\\");
}
bool removeDir(const QString &dirName, bool force = false)
{
    bool result = true;
    QDir dir(dirName);
    if (dir.exists(dirName))
    {
        Q_FOREACH (QFileInfo info,
                   dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files,
                                     QDir::DirsFirst))
        {
            if (info.isDir())
            {
                result = removeDir(info.absoluteFilePath());
            }
            else
            {
                result = QFile::remove(info.absoluteFilePath());
            }
            if (!result)
            {
                qDebug() << "Could not remove:" << info.absoluteFilePath();
                if (force)
                    continue;
                return result;
            }
            qDebug() << "Removed:" << info.absoluteFilePath();
        }
        result = dir.rmdir(dirName);
        if (!result)
        {
            qDebug() << "Could not remove:" << dirName;
            if (force)
                return true;
            return result;
        }
        qDebug() << "Removed:" << dirName;
    }
    return result;
}
int main(int argc, char **argv)
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(utf8LogHandler);
    QCommandLineParser parser;
    parser.addHelpOption();
    QCommandLineOption rmdirOpt(QStringList() << "rmdir", "Remove dir recursively.", "dirPath");
    parser.addOption(rmdirOpt);
    QCommandLineOption scandirOpt(QStringList() << "scandir", "Scan dir recursively.", "dirPath");
    parser.addOption(scandirOpt);
    QCommandLineOption envOpt(QStringList() << "env", "Show environment.");
    parser.addOption(envOpt);
    if (!parser.parse(qApp->arguments()))
    {
        parser.showHelp(-1);
        return (-1);
    }
    if (parser.isSet(rmdirOpt))
    {
        QString cmdArg = parser.value(rmdirOpt);
        qDebug() << "Removing:" << cmdArg;
        if (!removeDir(QFileInfo(cmdArg).absoluteFilePath(), true))
            return 1;
        return 0;
    }
    else if (parser.isSet(scandirOpt))
    {
        QString cmdArg = parser.value(scandirOpt);
        qDebug() << "Scanning:" << cmdArg;
        cmdArg = QFileInfo(cmdArg).absoluteFilePath();
        //  ファイルフィルタ
        QDir::Filters filters = QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot;
        // 対象フラグ
        QDirIterator::IteratorFlags flags = QDirIterator::Subdirectories;
        QDirIterator it(cmdArg, filters, flags);
        // QStringList realRepoList;
        QStringList realRepoList;
        QTextStream cout(stdout, QIODevice::WriteOnly);
        while (it.hasNext())
        {
            QString path = it.next();
            // qDebug() << path;
            QFileInfo info(path);
            cout << info.size() << " " << path << Qt::endl;
        }
        return 0;
    }
    else if (parser.isSet(envOpt))
    {
        QVariantMap m_env;
        // m_env["dir"] = qApp->applicationDirPath();
        m_env["temp"] = np(QDir::tempPath());
        m_env["prof"] = np(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
        m_env["docs"] = np(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
        m_env["repoRoot"] = np(m_env["docs"].toString() + "/.repo");
        m_env["msys2"] = np(m_env["prof"].toString() + "/.software/msys2");
        m_env["scoop"] = np(m_env["prof"].toString() + "/scoop");
        QJsonDocument result = QJsonDocument::fromVariant(m_env);
        std::cout << result.toJson().trimmed().toStdString() << std::endl << std::flush;
        return 0;
    }
    else
    {
        qDebug() << "Option not specified.";
        parser.showHelp(-1);
        return -1;
    }
    return 0;
}
