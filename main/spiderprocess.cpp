#include "spiderprocess.h"
#include "windowsutils.h"
SpiderProcess::SpiderProcess(SpiderProcCallback callback)
{
    auto uhomeName = g_core().selectedRepoName();
    QString uhomeDir = g_core().env()["docs"] + "/.repo/" + uhomeName;
    if (uhomeName.isEmpty())
    {
        uhomeDir = g_core().env()["docs"] + "/.repo";
        QDir docsDir = g_core().env()["docs"];
        docsDir.mkpath(".repo");
    }
    QString cmd2 = uhomeDir + "/cmd";
    QString pathAdded = np(cmd2);
    auto msys2Name = g_core().selectedMsys2Name();
    QString msys2Dir = "";
    if (msys2Name.isEmpty())
    {
        msys2Name = "(none)";
    }
    else
    {
        msys2Dir = np(g_core().env()["msys2"] + "/" + msys2Name); //QT_MSYS2_DIR
        //pathAdded += ";";
        //pathAdded += np(g_core().env()["msys2"] + "/" + msys2Name + "/mingw64/bin");
        //pathAdded += ";";
        //pathAdded += np(g_core().env()["msys2"] + "/" + msys2Name + "/usr/bin");
    }
    m_proc = new QProcess();
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_env = &env;
    if(msys2Name != "(none)")
    {
        env.insert("MSYS2_DIR", msys2Dir);
        env.insert("QT_MSYS2", "true");
        env.insert("QT_MSYS2_DIR", msys2Dir);
        env.insert("QT_MSYS2_ARCH", "amd64");
        env.insert("QT_MSYS2_STATIC", "true");
        env.insert("GOROOT", np(msys2Dir + "/mingw64/lib/go"));
    }
#if 0x0
    QStringList c_include_path = env.value("C_INCLUDE_PATH").split(":");
    c_include_path.prepend(np(uhomeDir + "/include"));
    env.insert("C_INCLUDE_PATH", c_include_path.join(";"));
    QStringList cplus_include_path = env.value("CPLUS_INCLUDE_PATH").split(":");
    cplus_include_path.prepend(np(uhomeDir + "/include"));
    env.insert("CPLUS_INCLUDE_PATH", cplus_include_path.join(";"));
#endif
    env.insert("UNCRUSTIFY_CONFIG", np(uhomeDir + "/.uncrustify.cfg"));
    QStringList wslenv = env.value("WSLENV").split(":");
    env.insert("WIN_HOME", np(uhomeDir));
    wslenv.append("WIN_HOME/p");
    SpiderSettings settings(g_core().env(), uhomeName);
    settings.settings().beginGroup("environmentVariable");
    auto keys = settings.settings().childKeys();
    foreach (QString key, keys)
    {
        env.insert(key, settings.settings().value(key).toString());
        wslenv.append(key);
    }
    settings.settings().endGroup();
    env.insert("HOME", np(uhomeDir));
    env.insert("PATH", pathAdded + ";" + g_core().env()["path"]);
    env.insert("REPO", uhomeName);
    env.insert("MSYS2", msys2Name);
    env.insert("WSLENV", wslenv.join(":"));
    m_proc->setWorkingDirectory(np(uhomeDir));
    callback(SpiderProcStage::PROC_SETUP, this);
    m_proc->setProcessEnvironment(env);
    QObject::connect(m_proc, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                     [this, callback](int exitCode, QProcess::ExitStatus exitStatus)
    {
        callback(SpiderProcStage::PROC_FINISH, this);
#if 0x0
        m_proc->deleteLater();
        this->deleteLater();
#endif
    });
}
SpiderProcess::~SpiderProcess()
{
    if (m_proc != nullptr)
        m_proc->deleteLater();
}
QProcess *SpiderProcess::proc()
{
    return m_proc;
}
QProcessEnvironment *SpiderProcess::env()
{
    return m_env;
}
void SpiderProcess::start()
{
    m_proc->start();
}
bool SpiderProcess::waitForFinished(int msecs)
{
    return m_proc->waitForFinished(msecs);
}
void SpiderProcess::startDetached()
{
    m_proc->startDetached();
}
