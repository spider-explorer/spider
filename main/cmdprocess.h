#ifndef CMDPROCESS_H
#define CMDPROCESS_H
#include <QtCore>
class CmdProcess : public QObject //: public QProcess
{
    Q_OBJECT
private:
    QMap<QString, QString> m_env;
    QString m_title;
    QString m_cmdLines;
    QString m_ext;
    QProcess m_proc;
public:
    CmdProcess(QMap<QString, QString> &env, QString title, QString cmdLines, QString ext);
    void run();
private slots:
    void handle_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void handle_errorOccurred(QProcess::ProcessError processError);
signals:
    void finished(int exitCode, QProcess::ExitStatus exitStatus);
    void errorOccurred(QProcess::ProcessError processError);
};
#endif // CMDPROCESS_H
