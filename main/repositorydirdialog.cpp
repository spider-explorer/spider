#include "repositorydirdialog.h"
#include "ui_repositorydirdialog.h"
#include "programdb.h"
RepositoryDirDialog::RepositoryDirDialog(QString rootPath, QWidget *parent)
    : QDialog(parent), m_rootPath(rootPath), ui(new Ui::RepositoryDirDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QFileInfo(rootPath).fileName());
    connect(ui->listView, &RepositoryDirView::rootPathChanged,
            [this](QString rootPath)
    {
        m_rootPath = rootPath;
        ui->label->setText(m_rootPath);
        if (m_rootPath.isEmpty())
        {
            ui->btnUpDir->setEnabled(false);
        }
        else
        {
            ui->btnUpDir->setEnabled(true);
        }
    });
    ui->listView->loadDir(m_rootPath);
    connect(ui->listView, &RepositoryDirView::fileDoubleClicked,
            [&](QString path)
    {
        qDebug() << "RepositoryDirDialog: "
                 "ファイルのダブルクリックを検知しました:"
                 << path;
        g_core().open_file(this, path);
    });
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested,
            [&](const QPoint &pos)
    {
        qDebug() << pos;
        const QPoint localPos = ui->listView->mapFrom(this, pos);
        QModelIndex index = ui->listView->indexAt(localPos);
        // int row = index.row();
        // qDebug() << "row=" << index.row();
        if (index.row() < 0)
            return;
        qDebug() << ui->listView->model().filePath(index);
        QString path = ui->listView->model().filePath(index);
        QMenu contextMenu("Context menu", this);
        QAction *actSetAsHome = new QAction(path, this);
        QObject::connect(actSetAsHome, &QAction::triggered, [this, path]()
        {
            qDebug() << "Exec:" << path;
        });
        contextMenu.addAction(actSetAsHome);
        contextMenu.exec(this->mapToGlobal(pos));
    });
}
RepositoryDirDialog::~RepositoryDirDialog()
{
    delete ui;
}
void RepositoryDirDialog::on_btnUpDir_clicked()
{
    ui->listView->upDir();
}
void RepositoryDirDialog::on_btnNyagos_clicked()
{
    g_core().open_nyagos(this, m_rootPath);
    this->accept();
}
void RepositoryDirDialog::on_btnBash_clicked()
{
    g_core().open_bash(this, m_rootPath);
    this->accept();
}
void RepositoryDirDialog::on_btnOffice_clicked()
{
    QString libreoffice = "soffice.exe";
    QProcess proc;
    proc.setProgram(ProgramDB().which(libreoffice));
    proc.setWorkingDirectory(m_rootPath);
    qDebug() << proc.startDetached();
}
