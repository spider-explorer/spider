#include "spidercore.h"
#include "jarchiver.h"
#include "cmdprocess.h"
#include "common.h"
#include "jinstaller.h"
#include "msys2dialog.h"
#include "programdb.h"
#include "projectchecker.h"
#include "spiderprocess.h"
#include "windowsutils.h"
#include "wslcore.h"
#include "jnetwork.h"
#include "junctionmanager.h"
static SpiderCore *s_core = nullptr;
static JsonSettings *s_settings = nullptr;
static QMutex *s_mutex = nullptr;
QString SpiderCore::prepareProgram(JsonSettings &softwareSettings, QString progName)
{
    JNetworkManager nm;
    QString urlString = softwareSettings.value(QString("software/%1/url").arg(progName)).toString();
    QString version = softwareSettings.value(QString("software/%1/version").arg(progName)).toString();
    QString ext = softwareSettings.value(QString("software/%1/ext").arg(progName)).toString();
    QString path = softwareSettings.value(QString("software/%1/path").arg(progName)).toString();
    m_splash.showMessage(QString("%1 を更新中(%2)...").arg(progName).arg(version), Qt::AlignLeft, Qt::white);
    QString dlPath = m_env["prof"] + QString("/.software/%1/%1-%2.%3").arg(progName).arg(version).arg(ext);
    QLocale locale;
    if (!QFileInfo(dlPath).exists())
    {
        QString parentPath = QFileInfo(dlPath).absolutePath();
        QDir(parentPath).removeRecursively();
        qDebug() << nm.getBatchAsFile(urlString, dlPath,
                               [this, &locale, progName, version](QNetworkReply *reply)
        {
            m_splash.showMessage(
                QString("%1 を更新中(%2)...ダウンロード中: %3")
                .arg(progName)
                .arg(version)
                .arg(locale.formattedDataSize(reply->bytesAvailable())),
                Qt::AlignLeft, Qt::white);
        });
    }
    m_splash.showMessage(
        QString("%1 を更新中(%2)...インストール中")
        .arg(progName)
        .arg(version),
        Qt::AlignLeft, Qt::white);
    QString installDir = m_env["prof"] + QString("/.software/%1/%2").arg(progName).arg(version);
    QString junctionDir = m_env["prof"] + QString("/.software/%1/current").arg(progName);
    if (!QFileInfo(installDir).exists())
    {
        qDebug() << extractZip(dlPath, installDir,
                               [this, &locale, progName, version](qint64 extractSizeTotal)
        {
            m_splash.showMessage(
                QString("%1 を更新中(%2)...インストール中: %3")
                .arg(progName)
                .arg(version)
                .arg(locale.formattedDataSize(extractSizeTotal)),
                Qt::AlignLeft, Qt::white);
        });
        JunctionManager().remove(junctionDir);
        JunctionManager().create(junctionDir, installDir);
    }
    if (!QFileInfo(junctionDir).exists())
    {
        JunctionManager().create(junctionDir, installDir);
    }
    if(!path.isEmpty())
    {
        QStringList pathList = path.split(";");
        for(int i=0; i<pathList.length(); i++)
        {
            QString pathElem = pathList[i];
            if(pathElem==".")
            {
                pathElem = junctionDir;
            }
            else
            {
                pathElem = junctionDir + pathElem;
            }
            pathList[i] = np(pathElem);
        }
        m_env["path"] = pathList.join(";") + ";" + m_env["path"];
    }
    return junctionDir;
}
QString SpiderCore::prepareWsl(QString distroName)
{
    JNetworkManager nm;
    m_splash.showMessage(distroName, Qt::AlignLeft, Qt::white);
    QString dlPath = m_env["prof"] + QString("/.software/wsl/%1.tar.7z").arg(distroName);
    QString urlString = QString("https://gitlab.com/javacommons/wsl-release/-/raw/main/%1.tar.7z").arg(distroName);
    qDebug() << nm.getBatchAsFile(urlString, dlPath,
    [](QNetworkReply *reply) {});
    QString installDir = m_env["prof"] + QString("/.software/wsl/%1").arg(distroName);
    if (!QFileInfo(installDir).exists())
    {
        qDebug() << extractZip(dlPath, installDir);
    }
    return installDir + QString("/%1.tar").arg(distroName);
}
SpiderCore::SpiderCore(QSplashScreen &splash, const QString &mainDllPath) : m_splash(splash), m_settings("spider")
{
    qDebug() << "SpiderCore::SpiderCore(1)";
    s_core = this;
    s_settings = &this->m_settings;
    s_mutex = &this->m_mutex;
    //
    QPixmap pixmap(":/one-moment-please.png");
    m_one_moment.setPixmap(pixmap);
    qDebug() << "SpiderCore::SpiderCore(2)";
    //
    QFileInfo mainDll(mainDllPath);
    m_env["dir"] = mainDll.absolutePath();
    //m_env["dir"] = mainDll.path();
    m_env["temp"] = m_env["dir"] + "/temp";
    m_env["prof"] = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    m_env["docs"] = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    m_env["repoRoot"] = m_env["docs"] + "/.repo";
    m_env["msys2"] = m_env["prof"] + "/.software/msys2";
    qDebug() << "SpiderCore::SpiderCore(3)";
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    m_env["path"] = env.value("PATH");
    //
    // prepareScoop();
    m_env["scoop"] = m_env["prof"] + "/scoop";
    m_env["path"] = np(m_env["scoop"] + "/shims") + ";" + m_env["path"];
    //
    m_env["path"] = np(m_env["dir"]) + ";" + m_env["path"];
    //
    m_env["path"] = np(m_env["dir"] + "/cmd") + ";" + m_env["path"];
    //
    m_env["path"] = np(m_env["dir"] + "/spiderbrowser") + ";" + m_env["path"];
    qDebug() << "SpiderCore::SpiderCore(4)";
    //
    QUrl softwareUrl("https://gitlab.com/spider-explorer/spider-software/-/raw/main/spider-software.json");
    JsonSettings softwareSettings(softwareUrl);
    qDebug() << "SpiderCore::SpiderCore(5)";
    QStringList appList = softwareSettings.value("software").toMap().keys();
    for(int i=0; i<appList.size(); i++)
    {
        if(appList[i]=="git") continue;
        prepareProgram(softwareSettings, appList[i]);
    }

    //
    ////QString sevenzip_dir = prepareProgram(softwareSettings, "7zip");
    //
    QString git_dir = prepareProgram(softwareSettings, "git");
    {
        QProcess gitProc;
        gitProc.setProgram(git_dir + "/bin/git.exe");
        gitProc.setArguments(QStringList() << "config"
                             << "--system"
                             << "core.autocrlf"
                             << "input");
        gitProc.start();
        gitProc.waitForFinished();
        gitProc.setArguments(QStringList() << "config"
                             << "--system"
                             << "credential.helper"
                             << "manager-core");
        gitProc.start();
        gitProc.waitForFinished();
    }
#if 0x0
    //
    QString wix_dir = prepareProgram(softwareSettings, "wixtoolset");
    //
    QString nyagos_dir = prepareProgram(softwareSettings, "nyagos");
    //
    QString wt_dir = prepareProgram(softwareSettings, "windows-terminal");
    //
    QString astyle_dir = prepareProgram(softwareSettings, "astyle");
    ////m_env["path"] = np(astyle_dir + "/bin") + ";" + m_env["path"];
    //
    QString uncrustify_dir = prepareProgram(softwareSettings, "uncrustify");
    ////m_env["path"] = np(uncrustify_dir + "/bin") + ";" + m_env["path"];
    //
    QString make_dir = prepareProgram(softwareSettings, "make");
    ////m_env["path"] = np(make_dir + "/bin") + ";" + m_env["path"];
    //
    QString cmake_dir = prepareProgram(softwareSettings, "cmake");
    ////m_env["path"] = np(cmake_dir + "/bin") + ";" + m_env["path"];
    //
    QString premake_dir = prepareProgram(softwareSettings, "premake");
    ////m_env["path"] = np(premake_dir) + ";" + m_env["path"];
    //
    QString deno_dir = prepareProgram(softwareSettings, "deno");
    ////m_env["path"] = np(deno_dir) + ";" + m_env["path"];
    //
    QString vscode_dir = prepareProgram(softwareSettings, "vscode");
    ////m_env["path"] = np(vscode_dir + "/bin") + ";" + np(vscode_dir) + ";" + m_env["path"];
    m_env["vscode"] = vscode_dir + "/Code.exe";
    //
    QString geany_dir = prepareProgram(softwareSettings, "geany");
    ////m_env["path"] = np(geany_dir + "/bin") + ";" + m_env["path"];
    m_env["geany"] = geany_dir + "/bin/geany.exe";
    //
    QString file_dir = prepareProgram(softwareSettings, "file");
    ////m_env["path"] = np(file_dir) + ";" + m_env["path"];
    //
    QString foldersize_dir = prepareProgram(softwareSettings, "FolderSizePortable");
    //
    QString sqlitestudio_dir = prepareProgram(softwareSettings, "sqlitestudio");
    //
    QString rapidee_dir = prepareProgram(softwareSettings, "rapidee");
    //
    QString busybox_dir = prepareProgram(softwareSettings, "busybox");
    // notepad3-5.21.1129.1
    QString notepad3_dir = prepareProgram(softwareSettings, "notepad3");
    //
    QString smartgit_dir = prepareProgram(softwareSettings, "smartgit");
    // office_x64-7.3.2.7z
    QString office_dir = prepareProgram(softwareSettings, "office_x64");
    // curl-7.82.0_2
    QString curl_dir = prepareProgram(softwareSettings, "curl");
    // wget-1.21.3
    QString wget_dir = prepareProgram(softwareSettings, "wget");
    // wget-1.21.3
    QString sed_dir = prepareProgram(softwareSettings, "sed");
    // qownnotes-22.4.0.7z
    QString qownnotes_dir = prepareProgram(softwareSettings, "qownnotes");
    // AirExplorer-4.6.2.7z
    QString airexplorer_dir = prepareProgram(softwareSettings, "AirExplorer");
    // Everything-1.4.1.1015.x64.zip
    QString everything_dir = prepareProgram(softwareSettings, "Everything");
    // jq
    QString jq_dir = prepareProgram(softwareSettings, "jq");
#endif
    //
    QString ubuntuTar = prepareWsl("Ubuntu");
    m_env["ubuntuTar"] = ubuntuTar;
    qDebug() << ubuntuTar << QFileInfo(ubuntuTar).exists();
    m_watcher.setTopDir(m_env["repoRoot"]);
}
QSplashScreen &SpiderCore::splash()
{
    return m_splash;
}
QMap<QString, QString> &SpiderCore::env()
{
    return m_env;
}
JsonSettings &SpiderCore::settings()
{
    return m_settings;
}
QString SpiderCore::selectedRepoName()
{
    // MySettings settings;
    QString repo = g_settings().value("selected/repoName").toString();
    QString repoDir = m_env["docs"] + "/.repo/" + repo;
    if (!QDir(repoDir).exists())
    {
        g_settings().setValue("selected/repoName", "");
        return "";
    }
    return repo;
}
QString SpiderCore::selectedMsys2Name()
{
    // MySettings settings;
    QString msys2 = g_settings().value("selected/msys2Name").toString();
    QString msys2Dir = m_env["prof"] + "/.software/msys2/" + msys2;
    if (!QDir(msys2Dir).exists())
    {
        g_settings().setValue("selected/msys2Name", "");
        return "";
    }
    return msys2;
}
void SpiderCore::open_nyagos(QWidget *widget, QString path)
{
    if (path.isEmpty())
    {
        path =
            QFileDialog::getExistingDirectory(widget, "nyagosで開くディレクトリを選択してください", m_env["repoRoot"]);
        if (path.isEmpty())
        {
            return;
        }
    }
    auto uhomeName = this->selectedRepoName();
    auto msys2Name = this->selectedMsys2Name();
    m_one_moment.show();
    m_one_moment.showMessage("nyagosを起動中...");
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, path, uhomeName, msys2Name](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("wt.exe"));
            proc->proc()->setArguments(
                QStringList() /*<< "--focus"*/ << R"(nt)"
                << "--title"
                << QString("(Nyagos) %1 + %2")
                .arg(uhomeName.isEmpty() ? ".repo" : uhomeName)
                .arg(msys2Name.isEmpty() ? "(none)" : msys2Name)
                << "-d" << path << R"(nyagos.exe)");
            proc->proc()->setWorkingDirectory(path);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            m_one_moment.finish(widget);
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "nyagosを起動しました");
                // WindowsUtils::ShowWindow(proc->proc()->processId());
                // WindowsUtils::RestoreWindow(proc->proc()->processId());
                widget->showMinimized();
            }
            else
            {
                QMessageBox::information(widget, "確認", "nyagosの起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderCore::open_bash(QWidget *widget, QString path)
{
    if (path.isEmpty())
    {
        path = QFileDialog::getExistingDirectory(widget, "bashで開くディレクトリを選択してください", m_env["repoRoot"]);
        if (path.isEmpty())
        {
            return;
        }
    }
    auto uhomeName = this->selectedRepoName();
    auto msys2Name = this->selectedMsys2Name();
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, path, uhomeName, msys2Name](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("wt.exe"));
            proc->proc()->setArguments(QStringList() << "nt"
                                       << "--title"
                                       << QString("(Bash) %1 + %2")
                                       .arg(uhomeName.isEmpty() ? ".repo" : uhomeName)
                                       .arg(msys2Name.isEmpty() ? "(none)" : msys2Name)
                                       << "-d" << path << "busybox.exe"
                                       << "bash"
                                       << "-l"
                                       << "-c" << QString("cd '%1' && bash").arg(path));
            proc->proc()->setWorkingDirectory(path);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "bashを起動しました");
                widget->showMinimized();
            }
            else
            {
                QMessageBox::information(widget, "確認", "bashの起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderCore::open_file(QWidget *widget, QString path)
{
    QFileInfo info(path);
    qDebug() << info.suffix();
    QFile file(path);
    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = file.read(8000);
        bool isBinary = false;
        for (int i = 0; i < bytes.size(); i++)
        {
            if (bytes.at(i) == 0)
            {
                isBinary = true;
                break;
            }
        }
        if (info.suffix() == "7z" || info.suffix() == "gz" || info.suffix() == "xz" || info.suffix() == "tar" ||
                info.suffix() == "zip")
        {
            SpiderProcess *sproc = new SpiderProcess(
                [this, widget, path](SpiderProcStage stage, SpiderProcess *proc)
            {
                if (stage == SpiderProcStage::PROC_SETUP)
                {
                    proc->proc()->setProgram(ProgramDB().which("7zFM.exe"));
                    proc->proc()->setArguments(QStringList() << path);
                    proc->proc()->setWorkingDirectory(QFileInfo(path).absolutePath());
                }
                else if (stage == SpiderProcStage::PROC_FINISH)
                {
                    if (proc->proc()->exitCode() == 0)
                    {
                        // QMessageBox::information(widget,
                        // "確認", "7zFMを起動しました");
                    }
                    else
                    {
                        QMessageBox::information(widget, "確認", "7zFMの起動が失敗しました");
                    }
                    proc->deleteLater();
                }
            });
            sproc->start();
        }
        else if (info.suffix() == "c" || info.suffix() == "cpp" || info.suffix() == "cxx" ||
                 info.suffix() == "h" || info.suffix() == "hpp" || info.suffix() == "hxx" ||
                 info.suffix() == "mk" || info.suffix() == "make" ||
                 info.suffix() == "html" || info.suffix() == "htm" ||
                 info.suffix() == "js" || info.suffix() == "mjs")
        {
            this->develop_with_geany(widget, path);
        }
        else if (info.suffix() == "odb" || info.suffix() == "odf" || info.suffix() == "odg" || info.suffix() == "odp" ||
                 info.suffix() == "ods" || info.suffix() == "odt" ||
                 info.suffix() == "doc" || info.suffix() == "xls" || info.suffix() == "ppt" ||
                 info.suffix() == "docx" || info.suffix() == "xlsx" || info.suffix() == "pptx")
        {
            SpiderProcess *sproc = new SpiderProcess(
                [this, widget, path](SpiderProcStage stage, SpiderProcess *proc)
            {
                if (stage == SpiderProcStage::PROC_SETUP)
                {
                    proc->proc()->setProgram(ProgramDB().which("soffice.exe"));
                    proc->proc()->setArguments(QStringList() << path);
                    proc->proc()->setWorkingDirectory(QFileInfo(path).absolutePath());
                }
                else if (stage == SpiderProcStage::PROC_FINISH)
                {
                    if (proc->proc()->exitCode() == 0)
                    {
                        // QMessageBox::information(widget, "確認",
                        // "LibreOfficeを起動しました");
                    }
                    else
                    {
                        QMessageBox::information(widget, "確認", "LibreOfficeの起動が失敗しました");
                    }
                    proc->deleteLater();
                }
            });
            sproc->start();
        }
        else if (info.suffix() == "pro")
        {
            this->develop_with_qtcreator(widget, info.absoluteFilePath());
        }
        else if (info.suffix() == "lpr")
        {
            this->develop_with_lazarus(widget, info.absoluteFilePath());
        }
        else if (info.suffix() == "workspace" || info.suffix() == "project")
        {
            this->develop_with_codelite(widget, info.absoluteFilePath());
        }
        else if (isBinary)
        {
            // QMessageBox::information(widget, "確認", "バイナリファイルです");
            QUrl url = QUrl::fromLocalFile(path);
            if (!QDesktopServices::openUrl(url))
            {
                QMessageBox::information(widget, "確認", "このファイルは開けませんでした");
            }
        }
        else
        {
            // QMessageBox::information(widget, "確認", "テキストファイルです");
            QString notepad3 = ProgramDB().which("notepad3.exe");
            QProcess proc;
            proc.setProgram(notepad3);
            proc.setArguments(QStringList() << path);
            proc.setWorkingDirectory(QFileInfo(path).absolutePath());
            qDebug() << proc.startDetached();
        }
    }
}

void SpiderCore::open_notepad3(QWidget *widget, QString path)
{
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, path](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("notepad3.exe"));
            proc->proc()->setArguments(QStringList() << path);
            proc->proc()->setWorkingDirectory(QFileInfo(path).absolutePath());
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "notepad3を起動しました");
            }
            else
            {
                QMessageBox::information(widget, "確認", "notepad3の起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}

bool SpiderCore::check_system_qt_project(QWidget *widget, QString proFile)
{
    QFileInfo userInfo = proFile + ".user";
    if(userInfo.exists())
    {
        QFile userFile(userInfo.absoluteFilePath());
        if(userFile.open(QIODevice::ReadOnly))
        {
            QByteArray bytes = userFile.readAll();
            if(bytes.contains("(MSYS2)"))
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    QFileInfo sysQt = QStringLiteral("C:/Qt/Tools/QtCreator/bin/qtcreator.exe");
    if(!sysQt.exists()) return false;
    QMessageBox::StandardButton reply = QMessageBox::question(widget, "確認", QString("%1で開きますか?").arg(sysQt.absoluteFilePath()),
                                        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        return true;
    }
    return false;
}
void SpiderCore::develop_with_qtcreator(QWidget *widget, QString proFile)
{
    bool useSysQt = this->check_system_qt_project(widget, proFile);
    m_one_moment.show();
    m_one_moment.showMessage("QtCreatorを起動中...");
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, proFile, useSysQt](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            if(useSysQt)
            {
                proc->proc()->setProgram("C:/Qt/Tools/QtCreator/bin/qtcreator.exe");
                proc->proc()->setArguments(QStringList() << proFile);
            }
            else
            {
                proc->proc()->setProgram(R"(cmd.exe)");
                proc->proc()->setArguments(QStringList() << "/c"
                                           << "mingw64x.cmd"
                                           << "start"
                                           << "qtcreator" << proFile);
            }
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            m_one_moment.finish(widget);
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認", QString("QtCreatorを起動しました(%1)").arg(proFile));
                widget->showMinimized();
            }
            else
            {
                QMessageBox::information(widget, "確認", QString("QtCreatorの起動が失敗しました(%1)").arg(proFile));
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderCore::develop_with_lazarus(QWidget *widget, QString lprFile)
{
#if 0x0
#endif
}
void SpiderCore::develop_with_codelite(QWidget *widget, QString projFile)
{
#if 0x0
#endif
}
void SpiderCore::develop_with_geany(QWidget *widget, QString path)
{
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, path](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("geany.exe"));
            proc->proc()->setArguments(QStringList() << path);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認", QString("geanyを起動しました(%1)").arg(path));
                widget->showMinimized();
            }
            else
            {
                QMessageBox::information(widget, "確認", QString("geanyの起動が失敗しました(%1)").arg(path));
            }
            proc->deleteLater();
        }
    });
    sproc->startDetached();
}
void SpiderCore::open_qt_dir(QWidget *widget, QString path)
{
    ProjectChecker ck(path);
    if (ck.isQtProject(true))
    {
        QString proFile = ck.getQtProjectFile();
        if (!proFile.isEmpty())
        {
            this->develop_with_qtcreator(widget, proFile);
        }
        else
        {
            // QMessageBox::information(widget, "確認",
            // "Qtプロジェクトがありません");
        }
    }
    else
    {
        QMessageBox::information(widget, "確認", "Qtプロジェクトではありません");
    }
}
void SpiderCore::open_explorer(QWidget *widget, QString repoDir)
{
    auto uhomeName = this->selectedRepoName();
    auto msys2Name = this->selectedMsys2Name();
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, repoDir, uhomeName, msys2Name](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram("explorer.exe");
            proc->proc()->setArguments(QStringList() << np(repoDir));
            proc->proc()->setWorkingDirectory(repoDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "explorerを起動しました");
            }
            else
            {
                QMessageBox::information(widget, "確認", "explorerの起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
RecursiveFileSystemWatcher &SpiderCore::watcher()
{
    return m_watcher;
}
void SpiderCore::remove_repo(QWidget *widget, QString repoDir)
{
    QString repo = QFileInfo(repoDir).fileName();
    QString repoRoot = QFileInfo(repoDir).absolutePath();
    QDateTime dt = QDateTime::currentDateTime();
    QString tempPath = QDir::tempPath() + "/spider.deleting." + repo + "." + dt.toString("yyyy-MM-dd-hh-mm-ss");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(widget, "確認", QString("%1を削除しますか?").arg(repo),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
    {
        return;
    }
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("set -e") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("cd %1").arg(repoRoot) << Qt::endl;
    strm << QString("mv %1 %2").arg(repo, tempPath) << Qt::endl;
    strm << QString("cmd.exe /c rmdir /s /q '%1'").arg(np(tempPath)) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(m_env, QString("%1 を削除").arg(repo), cmdLines, ".sh");
    proc->run();
}
void SpiderCore::refresh_repo(QWidget *widget, QString repoDir)
{
    QString repo = QFileInfo(repoDir).fileName();
    QString repoRoot = QFileInfo(repoDir).absolutePath();
    QDateTime dt = QDateTime::currentDateTime();
    QString tempPath = QDir::tempPath() + "/spider.deleting." + repo + "." + dt.toString("yyyy-MM-dd-hh-mm-ss");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(widget, "確認", QString("%1を削除してクローンしなおしますか?").arg(repo),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
    {
        return;
    }
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("set -e") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("cd %1/.repo/%2").arg(g_core().env()["docs"]).arg(repo) << Qt::endl;
    strm << QString("url=`git config --get remote.origin.url`") << Qt::endl;
    strm << QString("cd %1/.repo").arg(g_core().env()["docs"]) << Qt::endl;
    strm << QString("mv %1 %2").arg(repo, tempPath) << Qt::endl;
    strm << QString("cmd.exe /c rmdir /s /q '%1'").arg(np(tempPath)) << Qt::endl;
    strm << QString("git clone --recursive $url %1").arg(repo) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("%1 を再取得").arg(repo), cmdLines, ".sh");
    proc->run();
}
void SpiderCore::open_msys2(QWidget *widget, QString msys2Dir, QString currentDir)
{
    qDebug() << "SpiderCore::open_msys2():" << msys2Dir;
    auto uhomeName = this->selectedRepoName();
    auto msys2Name = QFileInfo(msys2Dir).fileName(); // this->selectedMsys2Name();
    // QString repoDir = m_env["docs"] + "/.repo/" + uhomeName;
    if (currentDir.isEmpty())
    {
        currentDir = m_env["docs"] + "/.repo/" + uhomeName;
    }
    SpiderProcess *sproc = new SpiderProcess(
        [widget, currentDir, uhomeName, msys2Name](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->env()->insert("MSYS2", msys2Name);
            proc->proc()->setProgram(ProgramDB().which("wt.exe"));
            proc->proc()->setArguments(QStringList() << "nt"
                                       << "--title"
                                       << QString("(Msys2) %1 + %2")
                                       .arg(uhomeName.isEmpty() ? ".repo" : uhomeName)
                                       .arg(msys2Name.isEmpty() ? "(none)" : msys2Name)
                                       << "-d" << currentDir << "cmd.exe"
                                       << "/c"
                                       << "mingw.cmd");
            proc->proc()->setWorkingDirectory(currentDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "msys2を起動しました");
            }
            else
            {
                QMessageBox::information(widget, "確認", "msys2の起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderCore::install_msys2(QWidget *widget)
{
    Msys2Dialog dlg;
    if (!dlg.exec())
        return;
    QString msys2Name = dlg.name();
    QString archive = "msys2-base-x86_64-20220319.tar.xz";
    QString archive_file = np(QString("%1/%2").arg(g_core().env()["temp"]).arg(archive));
    QString archive_url = QString("https://gitlab.com/javacommons/widget01/-/raw/main/%1").arg(archive);
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("busybox pwd") << Qt::endl;
    strm << QString("cd /d %1").arg(np(g_core().env()["prof"])) << Qt::endl;
    strm << QString("if not exist %1 busybox wget -O %1 %2").arg(archive_file).arg(archive_url) << Qt::endl;
    strm << QString("if not exist .software\\msys2 mkdir .software\\msys2") << Qt::endl;
    strm << QString("cd .software\\msys2") << Qt::endl;
    strm << QString("if exist %1 rmdir /s /q %1").arg(msys2Name) << Qt::endl;
    strm << QString("mkdir %1").arg(msys2Name) << Qt::endl;
    strm << QString("busybox tar xvf %2 -C %1 --strip-components 1").arg(msys2Name).arg(archive_file) << Qt::endl;
    strm << QString("call %1\\msys2_shell.cmd -msys2 -defterm -here -no-start -c %2")
         .arg(msys2Name)
         .arg(QString("echo \"Installation Complete (%1)...\"").arg(msys2Name))
         << Qt::endl;
    strm << QString("cd %1\\etc").arg(msys2Name) << Qt::endl;
    strm << QString("if not exist bash.bashrc.orig copy bash.bashrc "
                    "bash.bashrc.orig")
         << Qt::endl;
    strm << QString("busybox sed -e \"s/^  export PS1=.*$/  export "
                    "PS1='(%1@$MSYSTEM) "
                    "\\\\w \\$ '/g\" bash.bashrc.orig > bash.bashrc")
         .arg(msys2Name)
         << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("%1 をインストール").arg(msys2Name), cmdLines, ".cmd");
    proc->run();
}
void SpiderCore::remove_msys2(QWidget *widget, QString name)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(widget, "確認", QString("%1を削除しますか?").arg(name),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
    {
        return;
    }
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("busybox pwd") << Qt::endl;
    strm << QString("cd /d %1").arg(np(g_core().env()["msys2"])) << Qt::endl;
    strm << QString("move %1 %1.deleting").arg(name) << Qt::endl;
    strm << QString("rmdir /s /q %1.deleting").arg(name) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("%1 を削除").arg(name), cmdLines, ".cmd");
    proc->run();
}
void SpiderCore::open_git_page(QWidget *widget, QString repoDir)
{
    // QString repoDir = m_env["docs"] + "/.repo/" + repo;
    QProcess proc;
    proc.setProgram(ProgramDB().which("git.exe"));
    proc.setArguments(QStringList() << "config"
                      << "--get"
                      << "remote.origin.url");
    proc.setWorkingDirectory(repoDir);
    proc.start();
    proc.waitForFinished();
    QDesktopServices::openUrl(QUrl(QString::fromLatin1(proc.readAll().trimmed())));
}
void SpiderCore::open_vscode(QWidget *widget, QString repoDir)
{
    SpiderProcess *sproc = new SpiderProcess(
        [widget, repoDir](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("Code.exe"));
            proc->proc()->setArguments(QStringList() << repoDir);
            proc->proc()->setWorkingDirectory(repoDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "vscodeを起動しました");
            }
            else
            {
                QMessageBox::information(widget, "確認", "vscodeの起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderCore::open_smartgit(QWidget *widget, QString repoDir)
{
    m_one_moment.show();
    m_one_moment.showMessage("nyagosを起動中...");
    SpiderProcess *sproc = new SpiderProcess(
        [this, widget, repoDir](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("smartgit.exe"));
            proc->proc()->setArguments(QStringList() << "--open" << repoDir);
            proc->proc()->setWorkingDirectory(repoDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            m_one_moment.finish(widget);
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(widget, "確認",
                // "smartgitを起動しました");
            }
            else
            {
                QMessageBox::information(widget, "確認", "smartgitの起動が失敗しました");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
SpiderCore &g_core()
{
    return *s_core;
}
JsonSettings &g_settings()
{
    return *s_settings;
}
QMutex &g_mutex()
{
    return *s_mutex;
}
