#include "test01dialog.h"
#include "ui_test01dialog.h"
#include "wslfilesystemmodel.h"
Test01Dialog::Test01Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Test01Dialog)
{
    ui->setupUi(this);
    ui->treeView->setModel(&m_model);
    connect(ui->widget, &ExplorerForm::signal_folderRequestedContextMenu,
            [this](QMenu &contextMenu, QString path)
    {
        QAction *actSetAsHome = new QAction(QString("%1 を選択").arg(path), this);
        QObject::connect(actSetAsHome, &QAction::triggered, [this, path]()
        {
            qDebug() << path;
        });
        contextMenu.addAction(actSetAsHome);
        return true;
    });
}
Test01Dialog::~Test01Dialog()
{
    delete ui;
}
