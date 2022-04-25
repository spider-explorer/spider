#include "recursivefilesystemwatcher.h"
#include <iostream>
RecursiveFileSystemWatcher::RecursiveFileSystemWatcher()
{
    connect(&m_emitTimer, &QTimer::timeout,
            [this]()
    {
        QMutexLocker locker(&m_mutex);
        QStringList list = m_modifiedDirs.values();
        foreach (QString path, list)
        {
            // qDebug() << "modified()" << path;
            emit modified(path);
        }
        m_modifiedDirs.clear();
    });
    m_emitTimer.start(1000);
    m_watcher.watch();
}
RecursiveFileSystemWatcher::~RecursiveFileSystemWatcher()
{
}
void RecursiveFileSystemWatcher::setTopDir(QString path)
{
    // qDebug() << "RecursiveFileSystemWatcher::setTopDir(begin):" << path;
    m_topDir = QFileInfo(path).absoluteFilePath();
    efsw::WatchID watchID = m_watcher.addWatch(m_topDir.toUtf8().toStdString().c_str(), this, true);
    // qDebug() << "RecursiveFileSystemWatcher::setTopDir(end):" << path;
}
void RecursiveFileSystemWatcher::handleFileAction(efsw::WatchID watchid, const std::string &dir,
        const std::string &filename, efsw::Action action,
        std::string oldFilename)
{
#if 0x0
    switch (action)
    {
    case efsw::Actions::Add:
        std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
        break;
    case efsw::Actions::Delete:
        std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
        break;
    case efsw::Actions::Modified:
        std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
        break;
    case efsw::Actions::Moved:
        std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")"
                  << std::endl;
        break;
    default:
        std::cout << "Should never happen!" << std::endl;
    }
#endif
    QMutexLocker locker(&m_mutex);
    QDir dTopDir(m_topDir);
    QString sDir = QString::fromUtf8(QByteArray::fromStdString(dir));
    QString sFileName = QString::fromUtf8(QByteArray::fromStdString(filename));
    sDir = sDir.replace("\\", "/");
    QString path = m_topDir + "/" + dTopDir.relativeFilePath(sDir).split("/")[0];
    if (!sDir.endsWith("/.git") && !sDir.contains("/.git/") && sFileName != ".git")
    {
        // qDebug() << action << sDir << sFileName;
        m_modifiedDirs.insert(path);
    }
}
