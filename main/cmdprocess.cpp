#include "cmdprocess.h"
#include "common.h"
#include "programdb.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
CmdProcess::CmdProcess(QMap<QString, QString> &env, QString title, QString cmdLines, QString ext)
    : m_env(env), m_title(title), m_cmdLines(cmdLines), m_ext(ext)
{
    QObject::connect(&m_proc, SIGNAL(finished(int,QProcess::ExitStatus)), this,
                     SLOT(handle_finished(int,QProcess::ExitStatus)));
    QObject::connect(&m_proc, SIGNAL(errorOccurred(QProcess::ProcessError)), this,
                     SLOT(handle_errorOccurred(QProcess::ProcessError)));
}
void CmdProcess::run()
{
    qDebug() << m_env;
    qDebug() << m_env["path"];
    QDateTime dt = QDateTime::currentDateTime();
    // QTemporaryFile tempFile(m_env["temp"] + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "-XXXXXX" + m_ext);
    QTemporaryFile tempFile(QDir::tempPath() + "/" + dt.toString("yyyy-MM-dd-hh-mm-ss") + "-XXXXXX" + m_ext);
    tempFile.setAutoRemove(false);
    if (tempFile.open())
    {
        qDebug() << tempFile.fileName();
        tempFile.write(m_cmdLines.toLocal8Bit());
        /// tempFile.write(QString("\ndel \"%~f0\"").toLocal8Bit());
        // tempFile.write(QString("\nexit /b 0").toLocal8Bit());
        tempFile.close();
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        // QString path = env.value("PATH");
        env.insert("HOME", np(m_env["temp"]));
        env.insert("PATH", np(m_env["dir"] + "/cmd") + ";" + m_env["path"]);
        m_proc.setProgram(ProgramDB().which("wt.exe"));
        m_proc.setProcessEnvironment(env);
        m_proc.setWorkingDirectory(np(m_env["docs"]));
        m_proc.setArguments(QStringList() << "nt"
                            << "-d" << m_env["docs"] << "--title" << m_title << "cmd.exe"
                            << "/c"
                            << "exec_and_delete.cmd" << np(tempFile.fileName()));
        m_proc.start();
    }
    if (tempFile.open())
    {
        QTextStream out(&tempFile);
        qDebug().noquote() << out.readAll();
        tempFile.close();
    }
}
void CmdProcess::handle_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "CmdProcess::handle_finished" << exitCode << exitStatus;
    qDebug() << m_proc.readAll();
    emit finished(exitCode, exitStatus);
    this->deleteLater();
}
void CmdProcess::handle_errorOccurred(QProcess::ProcessError processError)
{
    qDebug() << "CmdProcess::handle_errorOccurred()" << processError;
    qDebug() << m_proc.readAll();
    emit errorOccurred(processError);
    this->deleteLater();
}
