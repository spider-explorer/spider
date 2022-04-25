#include "msys2dialog.h"
#include "common.h"
#include "ui_msys2dialog.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
Msys2Dialog::Msys2Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Msys2Dialog)
{
    ui->setupUi(this);
    this->on_lineEdit_textChanged("");
}
Msys2Dialog::~Msys2Dialog()
{
    delete ui;
}
QString Msys2Dialog::name() const
{
    return ui->lineEdit->text();
}
void Msys2Dialog::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    QString userProfile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString msys2Path = userProfile + "/.software/msys2/" + arg1;
    ui->label->setText("");
    // QRegExp rx("^[a-z0-9_-]+$", Qt::CaseSensitivity::CaseSensitive);
    if (arg1.isEmpty())
    {
        ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(false);
        ui->label->setText("名前を入力してください");
    }
    else if (QDir(msys2Path).exists())
    {
        ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(false);
        ui->label->setText("既に存在する名前です");
    }
    else if (!isVaridFolderName(arg1))
    {
        ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(false);
        ui->label->setText("使えない文字がふくまれています");
    }
    else
    {
        ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(true);
    }
}
