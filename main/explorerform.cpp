#include "explorerform.h"
#include "common.h"
#include "favmanager.h"
#include "recursivefilelister.h"
#include "searchthread.h"
#include "spidercore.h"
#include "ui_explorerform.h"
#include "variantserializer.h"
ExplorerForm::ExplorerForm(QWidget *parent) : QWidget(parent), ui(new Ui::ExplorerForm)
{
    ui->setupUi(this);
    //
    ui->labelCurrFolder->setText("");
    //
    m_topToolbar = new QToolBar(this);
    // m_topToolbar->setStyleSheet("QToolButton:!hover {background-color:lightgray} QToolBar {background: rgb(30, 30,
    // 30)}");
    m_topToolbar->setStyleSheet("QToolButton:!hover {background-color:lightgray}");
    ui->horizontalTopToolbar->addWidget(m_topToolbar);
    QSpacerItem *topToolbarHoriSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    ui->horizontalTopToolbar->addItem(topToolbarHoriSpacer);
    //
    ui->treeView->horizontalScrollBar()->setEnabled(true);
    ui->treeView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QTimer::singleShot(0, [this]()
    {
        ui->treeView->resizeColumnToContents(0);
    });
    //
    connect(ui->treeView, &QTreeView::expanded,
            [this](const QModelIndex &index)
    {
        ui->treeView->resizeColumnToContents(index.column());
    });
    connect(ui->treeView, &QTreeView::collapsed,
            [this](const QModelIndex &index)
    {
        ui->treeView->resizeColumnToContents(index.column());
    });
    // ui->treeView->collapseAll();
    connect(ui->treeView, &QTreeView::clicked, this, &ExplorerForm::on_treeView_clicked);
    connect(ui->listView, &QListView::doubleClicked, this, &ExplorerForm::on_listView_doubleClicked);
    //
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        qDebug() << pos;
        QModelIndex index = ui->treeView->indexAt(pos);
        if (!index.isValid())
        {
            qDebug() << "(!index.isValid())";
            return;
        }
        this->on_treeView_clicked(index);
        QMenu contextMenu("Context menu", this);
        QString folder = m_dirModel.filePath(index);
        emit signal_folderRequestedContextMenu(contextMenu, folder);
        contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    });
#if 0x1
    //
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listView, &QListView::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        qDebug() << pos;
        QModelIndex index = ui->listView->indexAt(pos);
        if (!index.isValid())
        {
            qDebug() << "(!index.isValid())";
            return;
        }
        QMenu contextMenu("Context menu", this);
        QString file = m_fileModel.filePath(index);
        emit signal_fileRequestedContextMenu(contextMenu, file);
        contextMenu.exec(ui->listView->viewport()->mapToGlobal(pos));
    });
    //
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget, &QListWidget::customContextMenuRequested,
            [this](const QPoint &pos)
    {
        qDebug() << pos;
        QListWidgetItem *item = ui->listWidget->itemAt(pos);
        if (item == nullptr)
        {
            qDebug() << "(item == nullptr)";
            return;
        }
        QMenu contextMenu("Context menu", this);
        QString file = item->data(Qt::UserRole).toString();
        emit signal_fileRequestedContextMenu(contextMenu, file);
        contextMenu.exec(ui->listWidget->viewport()->mapToGlobal(pos));
    });
#endif
}
ExplorerForm::~ExplorerForm()
{
    delete ui;
}
void ExplorerForm::reloadFileList(bool force)
{
    if (!force && !ui->lineEdit->text().isEmpty())
        return;
    this->on_lineEdit_textChanged(ui->lineEdit->text());
}
void ExplorerForm::setRootDir(QString path)
{
    m_topDir = path;
    ui->labelCurrFolder->setText(path);
    // emit signal_folderChanged(path);
    m_dirModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs);
    m_dirModel.setRootPath(m_topDir);
    ui->treeView->setModel(&m_dirModel);
    ui->treeView->setRootIndex(m_dirModel.index(m_topDir));
    ui->treeView->hideColumn(1);
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    ui->listView->setModel(nullptr);
}
QString ExplorerForm::currentDir()
{
    return ui->labelCurrFolder->text();
}
void ExplorerForm::setCurrentDir(QString path, bool emitSignal)
{
    QModelIndex index = m_dirModel.index(path);
    if (!index.isValid())
        return;
    ui->treeView->setCurrentIndex(index);
    this->on_treeView_clicked(index);
    ui->labelCurrFolder->setText(path);
#if 0x0
    if (emitSignal)
        emit signal_folderChanged(path);
#else
    emit signal_folderChanged(path);
#endif
}
QString ExplorerForm::saveState()
{
    QByteArray splitter_1 = ui->splitter_1->saveState();
    QByteArray splitter_2 = ui->splitter_2->saveState();
    QVariantMap map;
    map["splitter_1"] = splitter_1;
    map["splitter_2"] = splitter_2;
    return VariantSerializer().serializeToString(map);
}
void ExplorerForm::restoreState(QString s)
{
    QVariantMap map = VariantSerializer().deserializeFromString(s).toMap();
    if (map.contains("splitter_1"))
    {
        ui->splitter_1->restoreState(map["splitter_1"].toByteArray());
    }
    if (map.contains("splitter_2"))
    {
        ui->splitter_2->restoreState(map["splitter_2"].toByteArray());
    }
}
QToolBar *ExplorerForm::topToolbar()
{
    return this->m_topToolbar;
}
void ExplorerForm::on_treeView_clicked(const QModelIndex &index)
{
    ui->treeView->resizeColumnToContents(index.column());
    QString path = m_dirModel.filePath(index);
    ui->labelCurrFolder->setText(path);
    emit signal_folderChanged(path);
    if (path == m_topDir || QFileInfo(path).absolutePath() == m_topDir)
    {
        // ui->treeView->collapse(m_dirModel.index(path));
        m_fileModel.setFilter(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    }
    else
    {
        m_fileModel.setFilter(QDir::NoDot | QDir::AllDirs | QDir::Files);
    }
    m_fileModel.setRootPath(path);
    ui->listView->setModel(&m_fileModel);
    ui->listView->setRootIndex(m_fileModel.index(path));
    this->on_lineEdit_textChanged(ui->lineEdit->text());
}
void ExplorerForm::on_listView_doubleClicked(const QModelIndex &index)
{
    QFileInfo info = m_fileModel.fileInfo(index);
    QString path1 = m_fileModel.filePath(index);
    QString path = info.absoluteFilePath();
    qDebug() << info << path1 << path;
    if (info.isDir())
    {
        ui->treeView->setCurrentIndex(m_dirModel.index(path));
        this->on_treeView_clicked(m_dirModel.index(path));
        //ui->labelCurrFolder->setText(path);
        //emit signal_folderChanged(path);
        if (m_fileModel.filePath(index).endsWith("/.."))
        {
            ui->treeView->collapse(m_dirModel.index(path));
        }
        if (path == m_topDir || QFileInfo(path).absolutePath() == m_topDir)
        {
            // ui->treeView->collapse(m_dirModel.index(path));
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
    else
    {
        if (lastDoubleClickedFile != path || QDateTime::currentDateTime() > lastDoubleClickedFileTime.addMSecs(500))
        {
            lastDoubleClickedFile = path;
            lastDoubleClickedFileTime = QDateTime::currentDateTime();
            emit signal_fileDoubleClicked(path);
        }
    }
}
void ExplorerForm::on_lineEdit_textChanged(const QString &arg1)
{
    QString dirPath = m_dirModel.filePath(ui->treeView->currentIndex());
    qDebug() << dirPath;
    if (dirPath.isEmpty())
        return;
    QStringList pathList;
    if (arg1.isEmpty())
    {
        pathList = FavManager().favList(dirPath);
    }
    else
    {
#if 0x0
        QRegExp rx(arg1, Qt::CaseInsensitive);
        pathList = RecursiveFileLister().listFiles(dirPath,
                   [&rx](QString path) -> bool
        {
            QString fileName = QFileInfo(path).fileName();
            return (rx.indexIn(fileName) != -1);
        });
#else
        // QMutexLocker locker(&g_mutex());
        SearchThread *thread = new SearchThread(dirPath, arg1);
        connect(thread, &SearchThread::searchFinished, this, &ExplorerForm::on__searchFinished);
        thread->start();
#endif
    }
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    foreach (QString path, pathList)
    {
        QDir dir(dirPath);
        QString display = dir.relativeFilePath(path);
        QListWidgetItem *item = new QListWidgetItem(display, ui->listWidget);
        item->setData(Qt::UserRole, path);
        m_dirModel.setListWidgetItemIcon(item, path);
        ui->listWidget->addItem(item);
    }
    ui->listWidget->blockSignals(false);
}
void ExplorerForm::on_splitter_1_splitterMoved(int pos, int index)
{
#if 0x0
    QByteArray state = ui->splitter_1->saveState();
    MySettings settings;
    g_settings().setBinary("form.explorer/split_1", state);
#endif
}
void ExplorerForm::on_splitter_2_splitterMoved(int pos, int index)
{
#if 0x0
    QByteArray state = ui->splitter_2->saveState();
    MySettings settings;
    g_settings().setBinary("form.explorer/split_2", state);
#endif
}
void ExplorerForm::on_btnSearchClear_clicked()
{
    ui->lineEdit->setText("");
}
void ExplorerForm::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString path = item->data(Qt::UserRole).toString();
    emit signal_fileDoubleClicked(path);
}
void ExplorerForm::on__searchFinished(qint64 threadId, QString topDir, QString rxString, QStringList pathList)
{
#if 0x0
    QMutexLocker locker(&g_mutex());
    if (threadId != SearchThread::count)
        return;
#else
    if (this->currentDir() != topDir)
        return;
    if (ui->lineEdit->text() != rxString)
        return;
#endif
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    foreach (QString path, pathList)
    {
        QDir dir(topDir);
        QString display = dir.relativeFilePath(path);
        QListWidgetItem *item = new QListWidgetItem(display, ui->listWidget);
        item->setData(Qt::UserRole, path);
        m_dirModel.setListWidgetItemIcon(item, path);
        ui->listWidget->addItem(item);
    }
    ui->listWidget->blockSignals(false);
}
