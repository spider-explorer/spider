#include "spiderform.h"
#include "cmdprocess.h"
#include "common.h"
#include "projectchecker.h"
#include "repodialog.h"
#include "spidercore.h"
#include "spiderprocess.h"
#include "ui_spiderform.h"
#include "wslcore.h"
SpiderForm::SpiderForm(QWidget *parent) : QWidget(parent), ui(new Ui::SpiderForm)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    // MySettings settings;
    QByteArray state1 = g_settings().binary("form.spider/split_1").toByteArray();
    if (state1.size() > 0)
    {
        ui->splitter_1->restoreState(state1);
    }
    //
    this->reloadNameList();
    connect(&m_reload_timer, &QTimer::timeout, [this]()
    {
        this->reloadNameList();
    });
    m_reload_timer.start(500);
    //
    this->checkGitStatusAll(true);
#if 0x0
    connect(&g_core().watcher(), &RecursiveFileSystemWatcher::modified,
            [this](QString path)
    {
        this->checkGitStatus(path, false);
    });
#else
    connect(&m_watch_timer, &QTimer::timeout, [this]()
    {
        this->checkGitStatusAll(false);
    });
    //m_watch_timer.start(2000);
#endif
    // m_repo_check_timer.start(3000);
    //
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        qDebug() << pos;
        QModelIndex index = ui->listWidget->indexAt(pos);
        if (!index.isValid())
        {
            qDebug() << "(!index.isValid())";
            return;
        }
        QListWidgetItem *item = ui->listWidget->currentItem();
        this->on_listWidget_itemClicked(item);
        QMenu contextMenu("Context menu", this);
        QString folder = g_core().env()["docs"] + "/.repo/" + item->text();
        emit signal_repoRequestedContextMenu(contextMenu, folder);
        contextMenu.exec(ui->listWidget->viewport()->mapToGlobal(pos));
    });
    //
    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_2, &QListWidget::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        qDebug() << pos;
        QModelIndex index = ui->listWidget_2->indexAt(pos);
        if (!index.isValid())
        {
            qDebug() << "(!index.isValid())";
            return;
        }
        QListWidgetItem *item = ui->listWidget_2->currentItem();
        QMenu contextMenu("Context menu", this);
        QString folder = g_core().env()["prof"] + "/.software/msys2/" + item->text();
        emit signal_msys2RequestedContextMenu(contextMenu, folder);
        contextMenu.exec(ui->listWidget_2->viewport()->mapToGlobal(pos));
    });
    //
    connect(ui->listWidget, &JListWidget::signal_mouseDoubleClick,
            [this](QListWidgetItem *item, Qt::MouseButton button, Qt::KeyboardModifiers modifiers)
    {
        QString repoDir = g_core().env()["docs"] + "/.repo/" + item->text();
        emit signal_repoDoubleClicked(repoDir, button, modifiers);
    });
}
SpiderForm::~SpiderForm()
{
    delete ui;
}
void SpiderForm::reloadNameList(bool force)
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
void SpiderForm::setCurrentFolder(QString path)
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString itemPath = g_core().env()["docs"] + "/.repo/" + item->text();
        // qDebug() << "path:" << path;
        // qDebug() << "itemPath:" << itemPath;
        if (path == itemPath || path.startsWith(itemPath + "/"))
        {
            item->setSelected(true);
        }
        else
        {
            item->setSelected(false);
        }
    }
}
void SpiderForm::checkGitStatus(QString path, bool full)
{
    if (path.endsWith(".deleting"))
        return;
    //qDebug() << "SpiderForm::checkGitStatus()" << path;
    QString repo = QFileInfo(path).fileName();
    QListWidgetItem *found = nullptr;
    QString repoDir;
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        repo = item->text();
        repoDir = g_core().env()["docs"] + "/.repo/" + repo;
        if (repoDir == path)
        {
            found = item;
            break;
        }
    }
    if (found == nullptr)
        return;
    SpiderProcess *sproc = new SpiderProcess(
        [full, found, repo, repoDir](SpiderProcStage stage, SpiderProcess *proc)
    {
        if (stage == SpiderProcStage::PROC_SETUP)
        {
            proc->proc()->setProgram("cmd.exe");
            proc->proc()->setArguments(QStringList() << "/c"
                                       << "busybox.exe"
                                       << "bash"
                                       << "-c" << (full ? "git-diff.sh --full" : "git-diff.sh"));
            proc->proc()->setWorkingDirectory(repoDir);
        }
        else if (stage == SpiderProcStage::PROC_FINISH)
        {
            QString status = QString::fromUtf8(proc->proc()->readAllStandardOutput()).trimmed();
            if (status.startsWith("[Remote Change]"))
            {
                found->setForeground(Qt::blue);
                found->setToolTip(status);
            }
            else if (status.startsWith("[Local change]"))
            {
                found->setForeground(Qt::red);
                found->setToolTip(status);
            }
            else
            {
                found->setForeground(Qt::black);
                found->setToolTip("");
            }
            proc->deleteLater();
        }
    });
    sproc->start();
}
void SpiderForm::checkGitStatusAll(bool full)
{
    for (int i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        QString repo = item->text();
        QString repoDir = g_core().env()["docs"] + "/.repo/" + repo;
        this->checkGitStatus(repoDir, full);
    }
}
void SpiderForm::on_splitter_1_splitterMoved(int pos, int index)
{
    QByteArray state = ui->splitter_1->saveState();
    // MySettings settings;
    g_settings().setBinary("form.spider/split_1", state);
}
void SpiderForm::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString repoDir = g_core().env()["docs"] + "/.repo/" + item->text();
    emit signal_repoClicked(repoDir);
}
void SpiderForm::on_btnCloneRepo_clicked()
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
void SpiderForm::on_btnResetSelection_clicked()
{
    m_reload_timer.stop();
    // MySettings settings;
    g_settings().remove("selected/repoName");
    g_settings().remove("selected/msys2Name");
    this->reloadNameList(true);
    m_reload_timer.start();
}
void SpiderForm::on_btnRepoStatus_clicked()
{
    this->checkGitStatusAll(true);
}
void SpiderForm::on_listWidget_2_itemDoubleClicked(QListWidgetItem *item)
{
    QString msys2 = item->text();
    QString path = g_core().env()["msys2"] + "/" + msys2;
    qDebug() << QFileInfo(path).exists();
    emit signal_msys2DoubleClicked(path);
}
void SpiderForm::on_btnMsys2Install_clicked()
{
    g_core().install_msys2(this);
}
