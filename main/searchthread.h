#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H
#include <QtCore>
class SearchThread : public QThread
{
    Q_OBJECT
public:
    explicit SearchThread(QString topDir, QString rxString, QObject *parent = nullptr);
    ~SearchThread();
    static qint64 count;
private:
    qint64 m_id;
    QString m_topDir;
    QString m_rxString;
protected:
    void run();
signals:
    void searchFinished(qint64 threadId, QString topDir, QString rxString, QStringList pathList);
};
#endif // SEARCHTHREAD_H
