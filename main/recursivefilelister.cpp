#include "recursivefilelister.h"
#include "spiderprocess.h"
RecursiveFileLister::RecursiveFileLister()
{
}
static QStringList search(QString dirName, RecursiveFileListerCallback callback, QSet<QString> &ignoredSet)
{
    // qDebug() << "search(top)" << dirName;
    QStringList result;
    QDir dir(dirName);
    if (dir.exists(dirName))
    {
        Q_FOREACH (QFileInfo info,
                   dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files,
                                     QDir::DirsFirst))
        {
            if (ignoredSet.contains(info.absoluteFilePath()))
            {
                qDebug() << "search(ignored)" << info;
                continue;
            }
            if (info.isDir())
            {
                // qDebug() << "search(dir)" << info;
                // if (info.fileName() != "node_modules")
                {
                    result.append(search(info.absoluteFilePath(), callback, ignoredSet));
                }
            }
            else
            {
                // qDebug() << "search(file)" << info;
                if (callback(info.absoluteFilePath()))
                {
                    result.append(info.absoluteFilePath());
                }
            }
        }
    }
    return result;
}
QStringList RecursiveFileLister::listFiles(QString topDir, RecursiveFileListerCallback callback)
{
    SpiderProcess *sproc = new SpiderProcess(
        [this, topDir](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram("cmd.exe");
            proc->proc()->setArguments(QStringList() << "/c"
                                       << "git-console-x86_64-static.exe"
                                       << "--action"
                                       << "ignored" << topDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
        }
    });
    sproc->start();
    sproc->waitForFinished();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(sproc->proc()->readAllStandardOutput());
    sproc->deleteLater();
    QStringList list = jsonDoc.toVariant().toStringList();
    QSet<QString> ignoredSet;
    foreach (QString x, list)
    {
        ignoredSet.insert(x);
    }
    qDebug() << "ignoredSet:" << ignoredSet;
    return search(QFileInfo(topDir).absoluteFilePath(), callback, ignoredSet);
}
