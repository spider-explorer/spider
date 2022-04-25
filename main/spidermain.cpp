#include "spidermain.h"
#include "cmdprocess.h"
#include "datetimemanager.h"
#include "envvardialog.h"
#include "favmanager.h"
#include "mainwindow.h"
#include "programdb.h"
#include "scoopsearchdialog.h"
#include "spidercore.h"
#include "spiderprocess.h"
#include "ui_spidermain.h"
SpiderMain::SpiderMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::SpiderMain)
{
    ui->setupUi(this);
    {
        QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();
        if (!QGuiApplication::styleHints()->showIsFullScreen()) {
            const QSize size = geometry.size() * 4 / 5;
            const QSize offset = (geometry.size() - size) / 2;
            const QPoint pos = geometry.topLeft() + QPoint(offset.width(), offset.height());
            geometry = QRect(pos, size);
        }
        this->setGeometry(geometry);
    }
    ui->toolBar->setStyleSheet(
        "QToolButton:!hover {background-color:lightgray} QToolBar {background: rgb(30, 30, 30)}");
    QFile version(":/spider2.version.txt");
    if (version.open(QIODevice::ReadOnly))
    {
        QByteArray versionBytes = version.readAll().trimmed();
        qDebug() << "version=" << versionBytes;
        this->setWindowTitle(QString("Spider Explorer v%1").arg(QString::fromLatin1(versionBytes)));
    }
    //
    QToolBar *explorerTopToolbar = ui->explorerWidget->topToolbar(); // new QToolBar(this);
    explorerTopToolbar->addAction(ui->actionChrome);
    explorerTopToolbar->addAction(ui->actionNotepad3);
    //
    // MySettings settings;
#if 0x0
    restoreGeometry(g_settings().binary("MainWindow/geometry").toByteArray());
    restoreState(g_settings().binary("MainWindow/windowState").toByteArray());
    ui->explorerWidget->restoreState(g_settings().value("ExplorerForm/state").toString());
#endif
    //
    connect(ui->explorerWidget, &ExplorerForm::signal_folderRequestedContextMenu,
            [this](QMenu &contextMenu, QString path)
    {
        QAction *actionNyagos = new QAction(QString("%1をnyagosで開く").arg(QFileInfo(path).fileName()), this);
        QObject::connect(actionNyagos, &QAction::triggered,
                         [this, path]()
        {
            g_core().open_nyagos(this, path);
        });
        contextMenu.addAction(actionNyagos);
        QAction *actionBash = new QAction(QString("%1をbashで開く").arg(QFileInfo(path).fileName()), this);
        QObject::connect(actionBash, &QAction::triggered,
                         [this, path]()
        {
            g_core().open_bash(this, path);
        });
        contextMenu.addAction(actionBash);
        QAction *actOpenVscode = new QAction(QString("%1 をvscodeで開く").arg(QFileInfo(path).fileName()), this);
        QObject::connect(actOpenVscode, &QAction::triggered, [this, path]()
        {
            g_core().open_vscode(this, path);
        });
        contextMenu.addAction(actOpenVscode);
    });
    //
    connect(ui->explorerWidget, &ExplorerForm::signal_fileRequestedContextMenu,
            [this](QMenu &contextMenu, QString path)
    {
        // MySettings settings;
        // AutoFavManager fav;
        QAction *actionOpen = new QAction(QString("%1を開く").arg(QFileInfo(path).fileName()), this);
        QObject::connect(actionOpen, &QAction::triggered,
                         [this, path]()
        {
            qDebug() << path;
            g_core().open_file(this, path);
        });
        contextMenu.addAction(actionOpen);
        if (FavManager().contains(path))
        {
            QAction *actionUnFavorite =
                new QAction(QString("%1をお気に入りから削除").arg(QFileInfo(path).fileName()), this);
            QObject::connect(actionUnFavorite, &QAction::triggered,
                             [this, path]()
            {
                qDebug() << path;
                FavManager().remove_fav(path);
                ui->explorerWidget->reloadFileList(false);
            });
            contextMenu.addAction(actionUnFavorite);
        }
        else
        {
            QAction *actionFavorite =
                new QAction(QString("%1をお気に入りに追加").arg(QFileInfo(path).fileName()), this);
            QObject::connect(actionFavorite, &QAction::triggered,
                             [this, path]()
            {
                qDebug() << path;
                FavManager().add_fav(path);
                ui->explorerWidget->reloadFileList(false);
            });
            contextMenu.addAction(actionFavorite);
        }
    });
    connect(ui->explorerWidget, &ExplorerForm::signal_folderChanged,
            [this](QString path)
    {
        qDebug() << path;
        ui->spiderWidget->setCurrentFolder(path);
    });
    connect(ui->spiderWidget, &SpiderForm::signal_repoClicked,
            [this](QString path)
    {
        ui->explorerWidget->setCurrentDir(path);
    });
    connect(ui->spiderWidget, &SpiderForm::signal_repoDoubleClicked,
            [this](QString path, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
    {
#if 0x1
        if (modifiers.testFlag(Qt::ControlModifier) == true)
        {
            g_core().open_qt_dir(this, path);
        }
        else if (modifiers.testFlag(Qt::ShiftModifier) == true)
        {
            g_core().open_bash(this, path);
        }
        else if (modifiers.testFlag(Qt::AltModifier) == true)
        {
            g_core().open_explorer(this, path);
        }
        else
        {
            g_core().open_nyagos(this, path);
        }
#endif
    });
    connect(ui->spiderWidget, &SpiderForm::signal_msys2DoubleClicked,
            [this](QString path)
    {
        g_core().open_msys2(this, path, ui->explorerWidget->currentDir());
    });
    ui->explorerWidget->setRootDir(g_core().env()["docs"] + "/.repo");
    //
    connect(ui->explorerWidget, &ExplorerForm::signal_fileDoubleClicked,
            [this](QString path)
    {
        qDebug() << "Received: ExplorerForm::signal_fileDoubleClicked";
        g_core().open_file(this, path);
    });
    //
    connect(
        ui->spiderWidget, &SpiderForm::signal_repoRequestedContextMenu,
        [this](QMenu &contextMenu, QString path)
    {
        QString repo = QFileInfo(path).fileName();
        qDebug() << repo;
        QString repoDir = path;
        // MySettings settings;
        //  QMenu contextMenu("Context menu", this);
        if (repo == g_settings().value("selected/repoName"))
        {
            QAction *actSetAsHome = new QAction(QString("%1 を選択解除").arg(repo), this);
            QObject::connect(actSetAsHome, &QAction::triggered,
                             [this, repo]()
            {
                // MySettings settings;
                g_settings().setValue("selected/repoName", "");
                ui->spiderWidget->reloadNameList(true);
            });
            contextMenu.addAction(actSetAsHome);
        }
        else
        {
            QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(repo), this);
            QObject::connect(actSetAsHome, &QAction::triggered,
                             [this, repo]()
            {
                // MySettings settings;
                g_settings().setValue("selected/repoName", repo);
                ui->spiderWidget->reloadNameList(true);
            });
            contextMenu.addAction(actSetAsHome);
        }
        {
            // QMenu *submenuOpenTerminal = contextMenu.addMenu(QString("%1 をターミナルで開く").arg(repo));
            QAction *actOpenNyagos = new QAction(QString("%1 をnyagosで開く").arg(repo), this);
            QObject::connect(actOpenNyagos, &QAction::triggered,
                             [this, repoDir]()
            {
                // qDebug() << QString("%1
                // をnyagosで開く").arg(repo);
                g_core().open_nyagos(this, repoDir);
            });
            // submenuOpenTerminal->addAction(actOpenNyagos);
            contextMenu.addAction(actOpenNyagos);
            QAction *actOpenBash = new QAction(QString("%1 をbashで開く").arg(repo), this);
            QObject::connect(actOpenBash, &QAction::triggered,
                             [this, repoDir]()
            {
                g_core().open_bash(this, repoDir);
            });
            // submenuOpenTerminal->addAction(actOpenBash);
            contextMenu.addAction(actOpenBash);
        }
        {
            QAction *actOpenVscode = new QAction(QString("%1 をvscodeで開く").arg(repo), this);
            QObject::connect(actOpenVscode, &QAction::triggered, [this, repoDir]()
            {
                g_core().open_vscode(this, repoDir);
            });
            contextMenu.addAction(actOpenVscode);
        }
        QAction *actRemove = new QAction(QString("%1 を削除").arg(repo), this);
        QObject::connect(actRemove, &QAction::triggered,
                         [this, repoDir]()
        {
            g_core().remove_repo(this, repoDir);
        });
        contextMenu.addAction(actRemove);
        if (QDir(repoDir + "/.git").exists())
        {
            QAction *actRefresh = new QAction(QString("%1 を再取得").arg(repo), this);
            QObject::connect(actRefresh, &QAction::triggered,
                             [this, repoDir]()
            {
                g_core().refresh_repo(this, repoDir);
            });
            contextMenu.addAction(actRefresh);
            QAction *actOpenSmartGit = new QAction(QString("%1 をsmartgitで開く").arg(repo), this);
            QObject::connect(actOpenSmartGit, &QAction::triggered,
                             [this, repoDir]()
            {
                g_core().open_smartgit(this, repoDir);
            });
            contextMenu.addAction(actOpenSmartGit);
            QAction *actOpenGitPage = new QAction(QString("%1 のウェブサイトを開く").arg(repo), this);
            QObject::connect(actOpenGitPage, &QAction::triggered,
                             [this, repoDir]()
            {
                g_core().open_git_page(this, repoDir);
            });
            contextMenu.addAction(actOpenGitPage);
        }
#if 0x0
        {
            QMenu *submenuOpenEditor = contextMenu.addMenu(QString("%1 をエディタで開く").arg(repo));
            QAction *actOpenEmacs = new QAction(QString("%1 をemacsで開く").arg(repo), this);
            QObject::connect(actOpenEmacs, &QAction::triggered, [this, repoDir]()
            {
                this->open_emacs(repoDir);
            });
            submenuOpenEditor->addAction(actOpenEmacs);
            QAction *actOpenVscode = new QAction(QString("%1 をvscodeで開く").arg(repo), this);
            QObject::connect(actOpenVscode, &QAction::triggered, [this, repoDir]()
            {
                this->open_vscode(repoDir);
            });
            submenuOpenEditor->addAction(actOpenVscode);
        }
        if (QDir(repoDir + "/.git").exists())
        {
            contextMenu.addAction(actOpenGitPage);
            QString shims = g_core().env()["prof"] + "/scoop/shims";
            QAction *actOpenSmartGit = new QAction(QString("%1 をsmartgitで開く").arg(item->text()), this);
            QObject::connect(actOpenSmartGit, &QAction::triggered,
                             [this, item]()
            {
                this->open_smartgit(item->text());
            });
            contextMenu.addAction(actOpenSmartGit);
        }
        if (QDir(repoDir + "/.git").exists())
        {
            QAction *actRefresh = new QAction(QString("%1 を再取得").arg(item->text()), this);
            QObject::connect(actRefresh, &QAction::triggered, [this, item]()
            {
                this->refresh_repo(item->text());
            });
            contextMenu.addAction(actRefresh);
        }
        QAction *actExplorer = new QAction(QString("%1 をエクスプローラーで表示").arg(item->text()), this);
        QObject::connect(actExplorer, &QAction::triggered, [this, item]()
        {
            this->open_explorer(item->text());
        });
        contextMenu.addAction(actExplorer);
        ProjectChecker ck(repoDir);
        // QString proFile = ck.getQtProjectFile();
        if (ck.isQtProject())
        {
            QAction *actDevelop1 = new QAction(QString("QtCreatorで開発"), this);
            QObject::connect(actDevelop1, &QAction::triggered,
                             [this, repoDir]()
            {
                ProjectChecker ck(repoDir);
                QString proFile = ck.getQtProjectFile();
                if (!proFile.isEmpty())
                {
                    develop_with_qtcreator(proFile);
                }
            });
            contextMenu.addAction(actDevelop1);
        }
        if (ck.isDartProject())
        {
            QAction *actDevelop2 = new QAction(QString("Android Studioで開発"), this);
            QObject::connect(actDevelop2, &QAction::triggered,
                             [this, repoDir]()
            {
                develop_with_android_studio(repoDir);
            });
            contextMenu.addAction(actDevelop2);
        }
        if (ck.isJavaProject())
        {
            QAction *actDevelop3 = new QAction(QString("ideaで開発"), this);
            QObject::connect(actDevelop3, &QAction::triggered, [this, repoDir]()
            {
                develop_with_idea(repoDir);
            });
            contextMenu.addAction(actDevelop3);
        }
#endif
    });
    connect(ui->spiderWidget, &SpiderForm::signal_msys2RequestedContextMenu,
            [this](QMenu &contextMenu, QString path)
    {
        QString msys2 = QFileInfo(path).fileName();
        QString msys2Dir = path;
        // MySettings settings;
        //  QMenu contextMenu("Context menu", this);
        if (msys2 == g_settings().value("selected/msys2Name"))
        {
            QAction *actSetAsHome = new QAction(QString("%1 を選択解除").arg(msys2), this);
            QObject::connect(actSetAsHome, &QAction::triggered,
                             [this, msys2]()
            {
                // MySettings settings;
                g_settings().setValue("selected/msys2Name", "");
                ui->spiderWidget->reloadNameList(true);
            });
            contextMenu.addAction(actSetAsHome);
        }
        else
        {
            QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(msys2), this);
            QObject::connect(actSetAsHome, &QAction::triggered,
                             [this, msys2]()
            {
                // MySettings settings;
                g_settings().setValue("selected/msys2Name", msys2);
                ui->spiderWidget->reloadNameList(true);
            });
            contextMenu.addAction(actSetAsHome);
        }
        QAction *action = new QAction(QString("%1 を削除").arg(msys2), this);
        QObject::connect(action, &QAction::triggered, [this, msys2]()
        {
            g_core().remove_msys2(this, msys2);
        });
        contextMenu.addAction(action);
    });
}
SpiderMain::~SpiderMain()
{
    delete ui;
}
void SpiderMain::closeEvent(QCloseEvent *event)
{
    // MySettings settings;
    qDebug() << "closeEvent()" << g_settings().fileName();
#if 0x0
    g_settings().beginGroup("MainWindow");
    g_settings().setBinary("geometry", saveGeometry());
    g_settings().setBinary("windowState", saveState());
    g_settings().endGroup();
    g_settings().setValue("ExplorerForm/state", ui->explorerWidget->saveState());
#endif
    g_settings().sync();
    QMainWindow::closeEvent(event);
}
void SpiderMain::on__signal_entry2()
{
    // QMessageBox::information(nullptr, "確認", "SpiderMain::on_signal_entry2()");
    //  https://stackoverflow.com/questions/6087887/bring-window-to-front-raise-show-activatewindow-don-t-work
    this->setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    this->activateWindow(); // for Windows
    this->raise();          // for MacOS
}
void SpiderMain::on_btnNyagos_clicked()
{
#if 0x0
    QString path = ui->explorerWidget->currentDir();
    g_core().open_nyagos(this, path);
#else
    g_core().open_nyagos(this);
#endif
}
void SpiderMain::on_btnBash_clicked()
{
#if 0x0
    QString path = ui->explorerWidget->currentDir();
    g_core().open_bash(this, path);
#else
    g_core().open_bash(this);
#endif
}
void SpiderMain::on_actionTest01_triggered()
{
    MainWindow *smain = new MainWindow();
    smain->show();
}
void SpiderMain::on_btnExplorer_clicked()
{
    QString path = ui->explorerWidget->currentDir();
    g_core().open_explorer(this, path);
}
void SpiderMain::on_actionChrome_triggered()
{
    QProcess proc;
    proc.setProgram(ProgramDB().which("chrome.exe"));
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionFolderSize_triggered()
{
    QString foldersize = ProgramDB().which("FolderSize64.exe");
    QProcess proc;
    proc.setProgram(foldersize);
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionRapidEE_triggered()
{
    QString rapidee = ProgramDB().which("rapidee.exe");
    QProcess proc;
    proc.setProgram(rapidee);
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionSQLiteStudio_triggered()
{
    QString sqlitestudio = ProgramDB().which("SQLiteStudio.exe");
    QProcess proc;
    proc.setProgram(sqlitestudio);
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionNotepad3_triggered()
{
    QString notepad3 = ProgramDB().which("notepad3.exe");
    QProcess proc;
    proc.setProgram(notepad3);
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionLibreOffice_triggered()
{
    QProcess proc;
    proc.setProgram(ProgramDB().which("soffice.exe"));
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionQtCreator_triggered()
{
    SpiderProcess *sproc = new SpiderProcess(
        [this](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram("cmd.exe");
            proc->proc()->setArguments(QStringList() << "/c"
                                       << "mingw64x.cmd"
                                       << "start"
                                       << "qtcreator");
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                QMessageBox::information(this, "確認", QString("QtCreatorを起動しました"));
            }
            else
            {
                QMessageBox::information(this, "確認", QString("QtCreatorの起動が失敗しました"));
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderMain::on_actionQOwnNotes_triggered()
{
    QProcess proc;
    proc.setProgram(ProgramDB().which("QOwnNotes.exe"));
    proc.setWorkingDirectory(np(g_core().env()["docs"]));
    qDebug() << proc.startDetached();
}
void SpiderMain::on_actionTest02_triggered()
{
    QDateTime dt = QDateTime::currentDateTime();
    QString s = DateTimeManager().toString(dt);
    qDebug() << dt << s;
    QDateTime dt2 = DateTimeManager().fromString(s);
    qDebug() << dt2;
}
void SpiderMain::on_actionManageScoop_triggered()
{
    ScoopSearchDialog dlg(g_core().env());
    if (dlg.exec())
    {
        QStringList nameList = dlg.nameList();
        QMap<QString, QVariantMap> statusMap = dlg.statusMap();
        QSet<QString> bucketSet;
        QStringList installList;
        QStringList uninstallList;
        foreach (QString name, nameList)
        {
            bool currentlyInstalled = (QDir(g_core().env()["prof"] + "/scoop/apps/" + name).exists());
            bool toBeInstalled = statusMap[name]["installed"].toBool();
            if (currentlyInstalled == toBeInstalled)
                continue;
            if (currentlyInstalled)
            {
                uninstallList.append(name);
            }
            else
            {
                installList.append(name);
                bucketSet.insert(statusMap[name]["bucket"].toString());
            }
        }
        if (installList.size() == 0 && uninstallList.size() == 0)
        {
            QMessageBox::information(this, "確認", "何もすることがありません");
            return;
        }
        qDebug() << "bucketSet" << bucketSet;
        qDebug() << "installList" << installList;
        qDebug() << "uninstallList" << uninstallList;
        QString buffer;
        QTextStream strm(&buffer);
        QStringList bucketList = bucketSet.values();
        strm << QString("#! bash -uvx") << Qt::endl;
        foreach (QString bucket, bucketList)
        {
            strm << QString("scoop bucket add %1").arg(bucket) << Qt::endl;
        }
        if (installList.size() > 0)
        {
            strm << QString("scoop install %1").arg(installList.join(" ")) << Qt::endl;
        }
        if (uninstallList.size() > 0)
        {
            strm << QString("scoop uninstall %1").arg(uninstallList.join(" ")) << Qt::endl;
        }
        strm << Qt::flush;
        QString cmdLines = *strm.string();
        CmdProcess *proc =
            new CmdProcess(g_core().env(), QString("scoopパッケージインストール/アンインストール"), cmdLines, ".sh");
        proc->run();
    }
}
void SpiderMain::on_actionManageEnvVars_triggered()
{
    auto repo = g_core().selectedRepoName();
    if (repo.isEmpty())
    {
        QMessageBox::information(this, "確認", "環境変数を記録するホームリポジトリを選択してください");
        return;
    }
    EnvVarDialog dlg(g_core().env(), repo);
    dlg.exec();
}
void SpiderMain::on_actionLazarus_triggered()
{
#if 0x0
#endif
}
void SpiderMain::on_actionAirExplorer_triggered()
{
#if 0x0
    QFileInfo program = ProgramDB().air_explorer();
    if (!program.exists())
    {
        QMessageBox::information(this, "確認", "AirExplorerがインストールされていません");
        return;
    }
#endif
    SpiderProcess *sproc = new SpiderProcess(
        [this](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("AirExplorer.exe"));
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(this, "確認", QString("AirExplorerを起動しました"));
            }
            else
            {
                QMessageBox::information(this, "確認", QString("AirExplorerの起動が失敗しました"));
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderMain::on_actionEverything_triggered()
{
    SpiderProcess *sproc = new SpiderProcess(
        [this](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram(ProgramDB().which("Everything.exe"));
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            if (proc->proc()->exitCode() == 0)
            {
                // QMessageBox::information(this, "確認", QString("Everythingを起動しました"));
            }
            else
            {
                QMessageBox::information(this, "確認", QString("Everythingの起動が失敗しました"));
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderMain::on_actionJoinMP4Files_triggered()
{
}
