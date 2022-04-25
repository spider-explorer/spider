#include "searchthread.h"
#include "recursivefilelister.h"
#include "spidercore.h"
qint64 SearchThread::count = 0;
SearchThread::SearchThread(QString topDir, QString rxString, QObject *parent)
    : QThread(parent), m_topDir(topDir), m_rxString(rxString)
{
    QMutexLocker locker(&g_mutex());
    m_id = ++SearchThread::count;
}
SearchThread::~SearchThread()
{
}
void SearchThread::run()
{
    try
    {
        QRegExp rx(m_rxString, Qt::CaseInsensitive);
        QStringList pathList = RecursiveFileLister().listFiles(m_topDir,
                               [this, rx](QString path) -> bool
        {
            {
                {
                    QMutexLocker locker(&g_mutex());
                    if (SearchThread::count > m_id)
                    {
                        qDebug() << "throwing";
                        throw QException();
                    }
                }
            }
            QString fileName = QFileInfo(path).fileName();
            return (rx.indexIn(fileName) != -1);
        });
        emit searchFinished(m_id, m_topDir, m_rxString, pathList);
    }
    catch (...)
    {
        {}
    }
    QTimer::singleShot(1000, [this]()
    {
        this->deleteLater();
    });
    // this->deleteLater();
}
