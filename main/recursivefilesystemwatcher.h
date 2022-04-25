#ifndef RECURSIVEFILESYSTEMWATCHER_H
#define RECURSIVEFILESYSTEMWATCHER_H
#include "efsw/efsw.hpp"
#include <QtCore>
class RecursiveFileSystemWatcher : public QObject, public efsw::FileWatchListener
{
    Q_OBJECT
public:
    RecursiveFileSystemWatcher();
    virtual ~RecursiveFileSystemWatcher();
    void setTopDir(QString path);
private:
    void handleFileAction(efsw::WatchID watchid, const std::string &dir, const std::string &filename,
                          efsw::Action action, std::string oldFilename) override;
signals:
    void modified(QString path);
private:
    QString m_topDir;
    efsw::FileWatcher m_watcher;
// QDateTime lastEmitTime;
    QMutex m_mutex;
    QSet<QString> m_modifiedDirs;
    QTimer m_emitTimer;
};
#endif // RECURSIVEFILESYSTEMWATCHER_H
