#include "mainwindow.h"
#include "cmdprocess.h"
#include "common.h"
#include "envvardialog.h"
#include "msys2dialog.h"
#include "programdb.h"
#include "projectchecker.h"
#include "qsettings_json.h"
#include "repodialog.h"
#include "repositorydirdialog.h"
#include "scoopsearchdialog.h"
#include "scoopsoftwaredialog.h"
#include "scoopsoftwarelist.h"
#include "spidermain.h"
#include "test01dialog.h"
#include "ui_mainwindow.h"
#include "variantserializer.h"
#include "wslcore.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    this->reloadExplorer(g_core().env()["docs"] + "/.repo");
#if 0x0
    m_topDir = g_core().env()["docs"] + "/.repo";
    m_dirModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    // m_dirModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    m_dirModel.setRootPath(m_topDir);
    ui->treeView->setModel(&m_dirModel);
    ui->treeView->setRootIndex(m_dirModel.index(m_topDir));
    // ui->treeView->setRootIsDecorated( true );
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    ui->labelFileModel->setText("");
#endif
    connect(&m_fileModel, &QFileSystemModel::rootPathChanged,
            [this](const QString &newPath)
    {
        qDebug() << newPath;
        ui->labelFileModel->setText(newPath);
    });
    connect(ui->listWidget, &JListWidget::signal_mouseDoubleClick, this, &MainWindow::doubleClickedButtons);
#if 0x0
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(showContextMenuForListWidget1(const QPoint&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(showContextMenuForListWidget2(const QPoint&)));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(showContextMenuForListWidget3(const QPoint&)));
#else
    {
        ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->listWidget, &QListWidget::customContextMenuRequested,
                [this](const QPoint &pos)
        {
            QModelIndex index = ui->listWidget->indexAt(pos);
            if (!index.isValid())
                return;
            QMenu contextMenu("Context menu", this);
            this->showContextMenuForListWidget1(ui->listWidget, contextMenu, index);
            contextMenu.exec(ui->listWidget->viewport()->mapToGlobal(pos));
        });
    }
    {
        ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->listWidget_2, &QListWidget::customContextMenuRequested,
                [this](const QPoint &pos)
        {
            QModelIndex index = ui->listWidget_2->indexAt(pos);
            if (!index.isValid())
                return;
            QMenu contextMenu("Context menu", this);
            this->showContextMenuForListWidget2(ui->listWidget_2, contextMenu, index);
            contextMenu.exec(ui->listWidget_2->viewport()->mapToGlobal(pos));
        });
    }
    {
        ui->listWidget_3->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(ui->listWidget_3, &QListWidget::customContextMenuRequested,
                [this](const QPoint &pos)
        {
            QModelIndex index = ui->listWidget_3->indexAt(pos);
            if (!index.isValid())
                return;
            QMenu contextMenu("Context menu", this);
            this->showContextMenuForListWidget3(ui->listWidget_3, contextMenu, index);
            contextMenu.exec(ui->listWidget_3->viewport()->mapToGlobal(pos));
        });
    }
#endif
    // this->ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    // // Drag & Drop Item Sort
#if 0x0
    QFile version(":/spider2.version.txt");
    if (version.open(QIODevice::ReadOnly))
    {
        QByteArray versionBytes = version.readAll().trimmed();
        qDebug() << "version=" << versionBytes;
        this->setWindowTitle(QString("Spider2 v%1").arg(QString::fromLatin1(versionBytes)));
    }
#else
#define STRING(str) #str
    QString version = STRING(SPIDER_VERSION);
    this->setWindowTitle(QString("Spider2 v%1").arg(version));
#endif
    this->reloadNameList();
    connect(&m_reload_timer, &QTimer::timeout, [this]()
    {
        this->reloadNameList();
    });
    m_reload_timer.start(500);
    // BinarySettings settings(g_core().env()["dir"] + "/test.bin.json",
    // BinaryFormat);
#if 0x0
    g_settings().setValue("test3", "value of test3");
    g_settings().beginGroup("test3");
    g_settings().setValue("test4", "value of test4");
    g_settings().endGroup();
#endif
    // qDebug() << g_settings().value("test3");
    this->statusBar()->showMessage("Hello! World!");
    // ui->listWidget->installEventFilter(&m_eventEater);
    //
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        QModelIndex index = ui->treeView->indexAt(pos);
        if (!index.isValid())
            return;
        QMenu contextMenu("Context menu", this);
        QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(m_dirModel.fileName(index)), this);
        QObject::connect(actSetAsHome, &QAction::triggered,
                         [this, index]()
        {
            qDebug() << m_dirModel.fileName(index) << m_dirModel.filePath(index);
        });
        contextMenu.addAction(actSetAsHome);
        contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    });
    //
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QListView::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        QModelIndex index = ui->listView->indexAt(pos);
        if (!index.isValid())
            return;
        QMenu contextMenu("Context menu", this);
        QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(m_fileModel.fileName(index)), this);
        QObject::connect(actSetAsHome, &QAction::triggered,
                         [this, index]()
        {
            qDebug() << m_fileModel.fileName(index) << m_fileModel.filePath(index);
        });
        contextMenu.addAction(actSetAsHome);
        contextMenu.exec(ui->listView->viewport()->mapToGlobal(pos));
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::reloadExplorer(QString topDir)
{
    m_topDir = topDir;
    m_dirModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    // m_dirModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    m_dirModel.setRootPath(m_topDir);
    ui->treeView->setModel(&m_dirModel);
    ui->treeView->setRootIndex(m_dirModel.index(m_topDir));
    // ui->treeView->setRootIsDecorated( true );
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
#if 0x0
    if (ui->treeView->currentIndex().isValid())
    {
        this->on_treeView_clicked(ui->treeView->currentIndex());
    }
    else
    {
        ui->labelFileModel->setText("");
        ui->listView->setModel(nullptr);
    }
#endif
    ui->treeView->setCurrentIndex(m_dirModel.index(0, 0));
    ui->labelFileModel->setText("");
    ui->listView->setModel(nullptr);
}
void MainWindow::reloadNameList(bool force)
{
    // MySettings settings;
    //  qDebug() << g_settings().fileName() << g_settings().format();
    //   ファイルフィルタ
    QDir::Filters filters = QDir::Dirs;
    // 対象フラグ
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    QDirIterator it0(g_core().env()["docs"] + "/.repo", filters, flags);
    // QStringList realRepoList;
    QStringList realRepoList;
    while (it0.hasNext())
    {
        QString dir = it0.next();
        QString fileName = QFileInfo(dir).fileName();
        if (dir.endsWith("/.") || dir.endsWith("/.."))
            continue;
        if (!isVaridFolderName(fileName))
            continue;
        ProjectChecker ck(dir);
        if (!ck.isVisible())
            continue;
#if 0x0
        if (ck.isGitDir())
        {
            // realRepoList.append("*" + QFileInfo(dir).fileName());
            realRepoList.insert(QFileInfo(dir).fileName(), true);
        }
        else
        {
            // realRepoList.append(QFileInfo(dir).fileName());
            realRepoList.insert(QFileInfo(dir).fileName(), false);
        }
#else
        realRepoList.append(QFileInfo(dir).fileName());
#endif
    }
    // qDebug() << realRepoList;
    updateListWidgetContent(ui->listWidget, &realRepoList, g_settings().value("selected/repoName").toString(), force);
    QDirIterator it2(g_core().env()["msys2"], filters, flags);
    // QStringList realMsys2List;
    QStringList realMsys2List;
    while (it2.hasNext())
    {
        QString dir = it2.next();
        QString fileName = QFileInfo(dir).fileName();
        if (dir.endsWith("/.") || dir.endsWith("/.."))
            continue;
        if (!isVaridFolderName(fileName))
            continue;
        if (!QFile(dir + "/msys2_shell.cmd").exists())
            continue;
        // realMsys2List.append(QFileInfo(dir).fileName());
        realMsys2List.append(QFileInfo(dir).fileName());
    }
    // qDebug() << realMsys2List;
    // QStringList currMsys2List;
    updateListWidgetContent(ui->listWidget_2, &realMsys2List, g_settings().value("selected/msys2Name").toString(),
                            force);
    //
    QStringList distroList = WslCore().distroList();
    QStringList realDistroList;
    foreach (QString distro, distroList)
    {
        realDistroList.append(distro);
    }
    updateListWidgetContent(ui->listWidget_3, &realDistroList, "", force);
}
QString MainWindow::selectedRepoName()
{
    // MySettings settings;
    QString repo = g_settings().value("selected/repoName").toString();
    QString repoDir = g_core().env()["docs"] + "/.repo/" + repo;
    qDebug() << repoDir << QDir(repoDir).exists();
    if (!QDir(repoDir).exists())
    {
        g_settings().setValue("selected/repoName", "");
        return "";
    }
    return repo;
}
QString MainWindow::selectedMsys2Name()
{
    // MySettings settings;
    QString msys2 = g_settings().value("selected/msys2Name").toString();
    QString msys2Dir = g_core().env()["prof"] + "/.software/msys2/" + msys2;
    if (!QDir(msys2Dir).exists())
    {
        g_settings().setValue("selected/msys2Name", "");
        return "";
    }
    return msys2;
}
void MainWindow::open_elvish(QString repo)
{
#if 0x0
#endif
}
void MainWindow::open_emacs(QString repoDir)
{
#if 0x0
#endif
}
void MainWindow::open_vscode(QString repoDir)
{
#if 0x0
#endif
}
void MainWindow::open_git_page(QString repo)
{
}
void MainWindow::open_smartgit(QString repo)
{
#if 0x0
#endif
}
void MainWindow::open_git_gui(QString repo)
{
#if 0x0
#endif
}
void MainWindow::clone_repo()
{
    RepoDialog dlg;
    if (!dlg.exec())
        return;
    QString repoUrl = dlg.url();
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("set -e") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("cd %1").arg(g_core().env()["docs"]) << Qt::endl;
    strm << QString("mkdir -p .repo") << Qt::endl;
    strm << QString("cd .repo") << Qt::endl;
    strm << QString("git clone --recursive %1").arg(repoUrl) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("リポジトリをクローン"), cmdLines, ".sh");
    proc->run();
}
void MainWindow::remove_repo(QString repo)
{
    QMessageBox::StandardButton reply;
    reply =
        QMessageBox::question(this, "確認", QString("%1を削除しますか?").arg(repo), QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes)
    {
        return;
    }
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("set -e") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("cd %1").arg(g_core().env()["docs"]) << Qt::endl;
    strm << QString("cd .repo") << Qt::endl;
    strm << QString("mv %1 %1.deleting").arg(repo) << Qt::endl;
    strm << QString("rm -rvf %1.deleting").arg(repo) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("%1 を削除").arg(repo), cmdLines, ".sh");
    proc->run();
}
void MainWindow::refresh_repo(QString repo)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "確認", QString("%1を削除してクローンしなおしますか?").arg(repo),
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
    strm << QString("mv %1 %1.deleting").arg(repo) << Qt::endl;
    strm << QString("rm -rvf %1.deleting").arg(repo) << Qt::endl;
    strm << QString("git clone --recursive $url %1").arg(repo) << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("%1 を再取得").arg(repo), cmdLines, ".sh");
    proc->run();
}
void MainWindow::open_explorer(QString repo)
{
#if 0x0
#endif
}
void MainWindow::develop_with_qtcreator(QString proFile)
{
#if 0x0
#endif
}
void MainWindow::develop_with_android_studio(QString repoDir)
{
#if 0x0
#endif
}
void MainWindow::develop_with_idea(QString repoDir)
{
#if 0x0
#endif
}
void MainWindow::remove_msys2(QString name)
{
    QMessageBox::StandardButton reply;
    reply =
        QMessageBox::question(this, "確認", QString("%1を削除しますか?").arg(name), QMessageBox::Yes | QMessageBox::No);
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
void MainWindow::install_or_update_flutter()
{
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("busybox pwd") << Qt::endl;
    strm << QString("cd /d %1").arg(np(g_core().env()["prof"])) << Qt::endl;
    strm << QString("if not exist .software mkdir .software") << Qt::endl;
    strm << QString("cd .software") << Qt::endl;
    strm << QString("if not exist flutter (") << Qt::endl;
    strm << QString("  git clone https://github.com/flutter/flutter.git -b stable") << Qt::endl;
    strm << QString("  envset.exe prepend PATH \"%1\"").arg(np(g_core().env()["prof"] + "/.software/flutter/bin"))
         << Qt::endl;
    strm << QString(") else (") << Qt::endl;
    strm << QString("  call \"%1\" upgrade").arg(np(g_core().env()["prof"] + "/.software/flutter/bin/flutter.bat"))
         << Qt::endl;
    strm << QString(")") << Qt::endl;
    strm << QString("call \"%1\" doctor").arg(np(g_core().env()["prof"] + "/.software/flutter/bin/flutter.bat"))
         << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("flutter をインストール/アップデート"), cmdLines, ".cmd");
    proc->run();
}
void MainWindow::install_msys2()
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
void MainWindow::showContextMenuForListWidget1(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index)
{
    // const QPoint localPos = ui->listWidget->mapFrom(this, pos);
    // QModelIndex index = ui->listWidget->indexAt(localPos);
    int row = index.row();
    qDebug() << "row=" << index.row();
    // if (index.row() < 0) return;
    QListWidgetItem *item = ui->listWidget->item(row);
    QString repo = item->text();
    qDebug() << repo;
    QString repoDir = g_core().env()["docs"] + "/.repo/" + repo;
    // MySettings settings;
    //  QMenu contextMenu("Context menu", this);
    QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(item->text()), this);
    QObject::connect(actSetAsHome, &QAction::triggered,
                     [this, item]()
    {
        // MySettings settings;
        g_settings().setValue("selected/repoName", item->text());
        this->reloadNameList(true);
    });
    if (item->text() == g_settings().value("selected/repoName"))
    {
        actSetAsHome->setEnabled(false);
    }
    contextMenu.addAction(actSetAsHome);
    {
        QMenu *submenuOpenTerminal = contextMenu.addMenu(QString("%1 をターミナルで開く").arg(repo));
        QAction *actOpenNyagos = new QAction(QString("%1 をnyagosで開く").arg(repo), this);
        QObject::connect(actOpenNyagos, &QAction::triggered,
                         [this, repoDir]()
        {
            // qDebug() << QString("%1
            // をnyagosで開く").arg(repo);
            g_core().open_nyagos(this, repoDir);
        });
        submenuOpenTerminal->addAction(actOpenNyagos);
        QAction *actOpenMsys2 = new QAction(QString("%1 をmsys2で開く").arg(repo), this);
        QObject::connect(actOpenMsys2, &QAction::triggered, [this, repo]()
        {
        });
        submenuOpenTerminal->addAction(actOpenMsys2);
        QAction *actOpenBash = new QAction(QString("%1 をbashで開く").arg(repo), this);
        QObject::connect(actOpenBash, &QAction::triggered, [this, repoDir]()
        {
            g_core().open_bash(this, repoDir);
        });
        submenuOpenTerminal->addAction(actOpenBash);
        QAction *actOpenElvish = new QAction(QString("%1 をelvishで開く").arg(repo), this);
        QObject::connect(actOpenElvish, &QAction::triggered, [this, repo]()
        {
            this->open_elvish(repo);
        });
        submenuOpenTerminal->addAction(actOpenElvish);
    }
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
        QAction *actOpenGitPage = new QAction(QString("%1 のウェブサイトを開く").arg(item->text()), this);
        QObject::connect(actOpenGitPage, &QAction::triggered, [this, item]()
        {
            this->open_git_page(item->text());
        });
        contextMenu.addAction(actOpenGitPage);
        QString shims = g_core().env()["prof"] + "/scoop/shims";
#if 0x0
        if (QFile(shims + "/smartgit.exe").exists())
        {
            QAction *actOpenSmartGit = new QAction(QString("%1 をsmartgitで開く").arg(item->text()), this);
            QObject::connect(actOpenSmartGit, &QAction::triggered,
                             [this, item]()
            {
                this->open_smartgit(item->text());
            });
            contextMenu.addAction(actOpenSmartGit);
        }
        else if (QFile(shims + "/git-gui.exe").exists())
        {
            QAction *actOpenGitGui = new QAction(QString("%1 をgit-guiで開く").arg(item->text()), this);
            QObject::connect(actOpenGitGui, &QAction::triggered, [this, item]()
            {
                this->open_git_gui(item->text());
            });
            contextMenu.addAction(actOpenGitGui);
        }
#else
        QAction *actOpenSmartGit = new QAction(QString("%1 をsmartgitで開く").arg(item->text()), this);
        QObject::connect(actOpenSmartGit, &QAction::triggered, [this, item]()
        {
            this->open_smartgit(item->text());
        });
        contextMenu.addAction(actOpenSmartGit);
#endif
    }
    QAction *actRemove = new QAction(QString("%1 を削除").arg(item->text()), this);
    QObject::connect(actRemove, &QAction::triggered, [this, item]()
    {
        this->remove_repo(item->text());
    });
    contextMenu.addAction(actRemove);
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
        QObject::connect(actDevelop2, &QAction::triggered, [this, repoDir]()
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
    // contextMenu.exec(this->mapToGlobal(pos));
}
void MainWindow::showContextMenuForListWidget2(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index)
{
    // const QPoint localPos = ui->listWidget_2->mapFrom(this, pos);
    // QModelIndex index = ui->listWidget_2->indexAt(localPos);
    int row = index.row();
    // qDebug() << "row=" << index.row();
    // if (index.row() < 0) return;
    QListWidgetItem *item = ui->listWidget_2->item(row);
    // MySettings settings;
    //  QMenu contextMenu("Context menu", this);
    QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(item->text()), this);
    QObject::connect(actSetAsHome, &QAction::triggered,
                     [this, item]()
    {
        // MySettings settings;
        g_settings().setValue("selected/msys2Name", item->text());
        this->reloadNameList(true);
    });
    if (item->text() == g_settings().value("selected/msys2Name"))
    {
        actSetAsHome->setEnabled(false);
    }
    contextMenu.addAction(actSetAsHome);
    QAction *actRemove = new QAction(QString("%1 を削除").arg(item->text()), this);
    QObject::connect(actRemove, &QAction::triggered, [this, item]()
    {
        this->remove_msys2(item->text());
    });
    contextMenu.addAction(actRemove);
    // contextMenu.exec(mapToGlobal(pos));
}
void MainWindow::showContextMenuForListWidget3(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index)
{
    // const QPoint localPos = ui->listWidget_3->mapFrom(this, pos);
    // QModelIndex index = ui->listWidget_3->indexAt(localPos);
    int row = index.row();
    // qDebug() << "row=" << index.row();
    // if (index.row() < 0) return;
    QListWidgetItem *item = ui->listWidget_3->item(row);
    QString distro = item->text();
    // QMenu contextMenu("Context menu", this);
    QAction *actionUnreg = new QAction(QString("%1を削除").arg(distro), this);
    QObject::connect(actionUnreg, &QAction::triggered,
                     [this, distro]()
    {
        QString installDir = g_core().env()["prof"] + QString("/.software/wsl/Ubuntu/%1").arg(distro);
        QString buffer;
        QTextStream strm(&buffer);
        strm << QString("#! bash -uvx") << Qt::endl;
        strm << QString("pwd") << Qt::endl;
        strm << QString("wsl -t %1 >/dev/null 2>&1").arg(distro) << Qt::endl;
        strm << QString("wsl --unregister %1 >/dev/null 2>&1").arg(distro) << Qt::endl;
        strm << QString("rm -rf %1").arg(installDir) << Qt::endl;
        QString menuDir =
            g_core().env()["prof"] +
            QString("/AppData/Roaming/Microsoft/Windows/Start Menu/Programs/%1").arg(distro);
        strm << QString("rm -rf '%1'").arg(menuDir) << Qt::endl;
        strm << Qt::flush;
        QString cmdLines = *strm.string();
        CmdProcess *proc =
            new CmdProcess(g_core().env(), QString("リポジトリをクローン"), cmdLines, ".sh");
        proc->run();
    });
    contextMenu.addAction(actionUnreg);
    QMap<QString, QString> guiAppMap = WslCore().guiAppMap(distro);
    // if(guiAppMap.size() == 0) return;
    QStringList keys = guiAppMap.keys();
    keys.sort();
    foreach (QString key, keys)
    {
        QString linkPath = guiAppMap[key];
        QMap<QString, QString> linkMap = WslCore().linkMap(linkPath);
        // qDebug() << key << linkPath;
        QAction *action = new QAction(key, this);
        QString iconPath = linkMap["IconLocation"];
        // qDebug() << iconPath << QFileInfo(iconPath).exists();
        if (!iconPath.isEmpty())
        {
            action->setIcon(QIcon(iconPath));
        }
        QObject::connect(action, &QAction::triggered,
                         [=]()
        {
            qDebug() << linkMap;
#if 0x0
#endif
        });
        contextMenu.addAction(action);
    }
    // contextMenu.exec(mapToGlobal(pos));
}
void MainWindow::doubleClickedButtons(QListWidgetItem *item, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
{
    qDebug() << "(1)";
    if (item == nullptr)
    {
        auto uhomeName = this->selectedRepoName();
        QString repoDir = g_core().env()["docs"] + "/.repo/" + uhomeName;
        if (modifiers.testFlag(Qt::ShiftModifier) == true)
        {
            g_core().open_nyagos(this, repoDir);
        }
        else
        {
            // this->open_bash(uhomeName);
            RepositoryDirDialog dlg(repoDir);
            dlg.exec();
        }
        return;
    }
    qDebug() << "(2)";
    qDebug() << "button=" << button;
    qDebug() << "modifiers=" << modifiers;
    QString repo = item->text();
    QString repoDir = g_core().env()["docs"] + "/.repo/" + repo;
    if (modifiers.testFlag(Qt::ControlModifier) == true)
    {
        qDebug() << "(3)";
        ProjectChecker ck(repoDir);
        if (ck.isQtProject(true))
        {
            QString proFile = ck.getQtProjectFile();
            if (!proFile.isEmpty())
            {
                this->develop_with_qtcreator(proFile);
            }
            else
            {
                // QMessageBox::information(this, "確認",
                // "Qtプロジェクトがありません");
            }
        }
        else
        {
            QMessageBox::information(this, "確認", "Qtプロジェクトではありません");
        }
    }
    else if (modifiers.testFlag(Qt::AltModifier) == true)
    {
        // this->open_explorer(repo);
#if 0x0
        RepositoryDirView *dirView = new RepositoryDirView(this);
        dirView->loadDir(repoDir);
        dirView->show();
#endif
        RepositoryDirDialog dlg(repoDir);
        dlg.exec();
    }
    else if (modifiers.testFlag(Qt::ShiftModifier) == true)
    {
        g_core().open_nyagos(this, repoDir);
    }
    else
    {
        // this->open_bash(repo);
        RepositoryDirDialog dlg(repoDir);
        dlg.exec();
    }
}
void MainWindow::on_btnCloneRepo_clicked()
{
    this->clone_repo();
}
void MainWindow::on_btnResetSelection_clicked()
{
#if 0x0
    m_reload_timer.stop();
    MySettings settings;
    g_settings().clear();
    g_settings().sync();
    this->reloadNameList(true);
    m_reload_timer.start();
#endif
}
void MainWindow::on_btnNyagos_clicked()
{
    // auto uhomeName = this->selectedRepoName();
    // QString repoDir = g_core().env()["docs"] + "/.repo/" + uhomeName;
    QString path = ui->labelFileModel->text();
    if (path.isEmpty())
    {
        QMessageBox::information(this, "確認", "フォルダを選択してください");
        return;
    }
    g_core().open_nyagos(this, path);
}
void MainWindow::on_btnMsys2_clicked()
{
}
void MainWindow::on_btnBash_clicked()
{
    QString path = ui->labelFileModel->text();
    if (path.isEmpty())
    {
        QMessageBox::information(this, "確認", "フォルダを選択してください");
        return;
    }
    g_core().open_bash(this, path);
}
void MainWindow::on_btnElvish_clicked()
{
    auto uhomeName = this->selectedRepoName();
    this->open_elvish(uhomeName);
}
void MainWindow::on_btnFlutterInstall_clicked()
{
    this->install_or_update_flutter();
}
void MainWindow::on_btnMsys2Install_clicked()
{
    this->install_msys2();
}
void MainWindow::on_btnEnvVarManage_clicked()
{
    auto repo = this->selectedRepoName();
    if (repo.isEmpty())
    {
        QMessageBox::information(this, "確認", "ホームリポジトリを選択してください");
        return;
    }
    EnvVarDialog dlg(g_core().env(), repo);
    dlg.exec();
}
void MainWindow::on_btnScoopManage_clicked()
{
    ScoopSoftwareDialog dlg(g_core().env());
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
void MainWindow::on_btnSettingsFolderOpen_clicked()
{
}
void MainWindow::on_actionChrome_triggered()
{
}
void MainWindow::on_actionFolderSize_triggered()
{
}
void MainWindow::on_actionRapidee_triggered()
{
}
void MainWindow::on_actionSQLiteStudio_triggered()
{
}
void MainWindow::on_actionExit_triggered()
{
    this->close();
}
void MainWindow::on_scoopSearch_clicked()
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
void MainWindow::on_actionQtCreator_triggered()
{
#if 0x0
#endif
}
void MainWindow::on_listWidget_3_itemDoubleClicked(QListWidgetItem *item)
{
#if 0x0
#endif
}
void MainWindow::on_listWidget_3_itemSelectionChanged()
{
    QListWidgetItem *current = ui->listWidget_3->currentItem();
    if (current == nullptr)
        return;
    QString distro = current->text();
    qDebug() << "Selected:" << distro;
    // QMap<QString, QString> guiAppMap = WslCore().guiAppMap(distro);
    // qDebug() << guiAppMap;
}
void MainWindow::on_actionNotepad3_triggered()
{
}
void MainWindow::on_btnUbuntuInstall_clicked()
{
    QString wslName = "wsl-test";
    QString installDir = g_core().env()["prof"] + QString("/.software/wsl/Ubuntu/%1").arg(wslName);
    qDebug() << installDir;
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("wsl -t %1 >/dev/null 2>&1").arg(wslName) << Qt::endl;
    strm << QString("wsl --unregister %1 >/dev/null 2>&1").arg(wslName) << Qt::endl;
    strm << QString("rm -rf %1").arg(installDir) << Qt::endl;
    strm << QString("mdkir -p %1").arg(installDir) << Qt::endl;
    strm << QString("wsl --import %1 %2 %3").arg(wslName).arg(installDir).arg(g_core().env()["ubuntuTar"]) << Qt::endl;
    strm
            << QString(
                R"***(powershell -Command "\$ErrorActionPreference = 'SilentlyContinue'; Function WSL-SetDefaultUser (\$distro, \$user) { Get-ItemProperty Registry::HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Lxss\*\ DistributionName | Where-Object -Property DistributionName -eq \$distro | Set-ItemProperty -Name DefaultUid -Value ((wsl -d \$distro -u \$user -e id -u) | Out-String); }; WSL-SetDefaultUser %1 user")***")
            .arg(wslName)
            << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("Ubuntuをインストール"), cmdLines, ".sh");
    proc->run();
}
void MainWindow::on_btnUbuntuImport_clicked()
{
    auto uhomeName = this->selectedRepoName();
    QString uhomeDir = g_core().env()["docs"] + "/.repo/" + uhomeName;
    QString fileName = QFileDialog::getOpenFileName(this, "Ubuntuインポート ファイル選択", uhomeDir,
                       tr("Tar files(*.tar);;All Files(*.*)"));
    qDebug() << fileName;
    QString wslName = QFileInfo(fileName).completeBaseName();
    QString installDir = g_core().env()["prof"] + QString("/.software/wsl/Ubuntu/%1").arg(wslName);
    qDebug() << installDir;
    QString buffer;
    QTextStream strm(&buffer);
    strm << QString("#! bash -uvx") << Qt::endl;
    strm << QString("pwd") << Qt::endl;
    strm << QString("wsl -t %1 >/dev/null 2>&1").arg(wslName) << Qt::endl;
    strm << QString("wsl --unregister %1 >/dev/null 2>&1").arg(wslName) << Qt::endl;
    strm << QString("rm -rf %1").arg(installDir) << Qt::endl;
    strm << QString("mdkir -p %1").arg(installDir) << Qt::endl;
    strm << QString("wsl --import %1 %2 %3").arg(wslName).arg(installDir).arg(fileName) << Qt::endl;
    strm
            << QString(
                R"***(powershell -Command "\$ErrorActionPreference = 'SilentlyContinue'; Function WSL-SetDefaultUser (\$distro, \$user) { Get-ItemProperty Registry::HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Lxss\*\ DistributionName | Where-Object -Property DistributionName -eq \$distro | Set-ItemProperty -Name DefaultUid -Value ((wsl -d \$distro -u \$user -e id -u) | Out-String); }; WSL-SetDefaultUser %1 user")***")
            .arg(wslName)
            << Qt::endl;
    strm << Qt::flush;
    QString cmdLines = *strm.string();
    CmdProcess *proc = new CmdProcess(g_core().env(), QString("Ubuntuをインポート"), cmdLines, ".sh");
    proc->run();
}
void MainWindow::on_actionLibreOffice_triggered()
{
}
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString path = g_core().env()["docs"] + "/.repo/" + item->text();
    ui->treeView->setCurrentIndex(m_dirModel.index(path));
    m_fileModel.setFilter(QDir::NoDot | QDir::AllDirs | QDir::Files);
    m_fileModel.setRootPath(path);
    ui->listView->setModel(&m_fileModel);
    ui->listView->setRootIndex(m_fileModel.index(path));
}
void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    // QString rootDir = g_core().env()["docs"] + "/.repo";
    QString path = m_dirModel.filePath(index);
    if (path == m_topDir || QFileInfo(path).absolutePath() == m_topDir)
    {
        ui->treeView->collapse(m_dirModel.index(path));
        m_fileModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    }
    else
    {
        m_fileModel.setFilter(QDir::NoDot | QDir::AllDirs | QDir::Files);
    }
    m_fileModel.setRootPath(path);
    ui->listView->setModel(&m_fileModel);
    ui->listView->setRootIndex(m_fileModel.index(path));
}
void MainWindow::on_listView_doubleClicked(const QModelIndex &index)
{
    /// QString rootDir = g_core().env()["docs"] + "/.repo";
    QFileInfo info = m_fileModel.fileInfo(index);
    QString path1 = m_fileModel.filePath(index);
    QString path = info.absoluteFilePath();
    qDebug() << info << path1 << path;
    if (info.isDir())
    {
        ui->treeView->setCurrentIndex(m_dirModel.index(path));
        if (path == m_topDir || QFileInfo(path).absolutePath() == m_topDir)
        {
            ui->treeView->collapse(m_dirModel.index(path));
            m_fileModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
        }
        else
        {
            m_fileModel.setFilter(QDir::NoDot | QDir::AllDirs | QDir::Files);
        }
        m_fileModel.setRootPath(path);
        ui->listView->setModel(&m_fileModel);
        ui->listView->setRootIndex(m_fileModel.index(path));
    }
}
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (index == 0)
    {
        this->reloadExplorer(g_core().env()["docs"] + "/.repo");
    }
    else
    {
        // this->reloadExplorer(R"(\\wsl.localhost)");
        this->reloadExplorer(g_core().env()["prof"] + "/.software/wsl/Ubuntu");
    }
}
void MainWindow::on_actionTest01_triggered()
{
#if 0x0
    Test01Dialog dlg;
    dlg.exec();
#else
    SpiderMain *smain = new SpiderMain();
    smain->show();
#endif
}
