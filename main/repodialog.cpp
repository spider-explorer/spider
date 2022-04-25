#include "repodialog.h"
#include "ui_repodialog.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
RepoDialog::RepoDialog(QWidget *parent) : QDialog(parent), ui(new Ui::RepoDialog)
{
    ui->setupUi(this);
    this->on_lineEdit_textChanged("");
}
RepoDialog::~RepoDialog()
{
    delete ui;
}
QString RepoDialog::url() const
{
    return m_url;
}
#if 0x0
QString RepoDialog::name() const
{
    return ui->lineEdit->text();
}
#endif
void RepoDialog::on_lineEdit_textChanged(const QString &arg1)
{
    QRegExp rx("^[a-z0-9_-]+$", Qt::CaseSensitivity::CaseInsensitive);
    if (arg1.isEmpty())
    {
        m_url = "";
    }
    else if (rx.exactMatch(arg1))
    {
        m_url = "https://gitlab.com/javacommons/" + arg1;
    }
    else
    {
        m_url = arg1;
    }
    ui->lblUrl->setText(m_url);
}
