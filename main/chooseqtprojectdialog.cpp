#include "chooseqtprojectdialog.h"
#include "ui_chooseqtprojectdialog.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
ChooseQtProjectDialog::ChooseQtProjectDialog(QString dir, QStringList proList, QWidget *parent)
    : QDialog(parent), m_proList(proList), m_dir(dir), ui(new Ui::ChooseQtProjectDialog)
{
    ui->setupUi(this);
#if 0x0
    ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(false);
    ui->listWidget->blockSignals(true);
    foreach (QString proFile, proList)
    {
        QDir dir(m_dir);
        QString display = dir.relativeFilePath(proFile);
        ui->listWidget->addItem(display);
    }
    ui->listWidget->blockSignals(false);
#else
    this->on_lineEdit_textChanged("");
#endif
}
ChooseQtProjectDialog::~ChooseQtProjectDialog()
{
    delete ui;
}
QString ChooseQtProjectDialog::proFile() const
{
    return m_proFile;
}
void ChooseQtProjectDialog::on_listWidget_itemSelectionChanged()
{
    // qDebug() << "currentRow=" << currentRow;
    // QListWidgetItem *current = ui->listWidget->item(currentRow);
    QListWidgetItem *current = ui->listWidget->selectedItems().at(0);
    m_proFile = current->data(Qt::UserRole).toString();
    ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(true);
}
void ChooseQtProjectDialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    // m_proFile = item->data(Qt::UserRole).toString();
    m_proFile = m_dir + "/" + item->text();
    this->accept();
}
void ChooseQtProjectDialog::on_lineEdit_textChanged(const QString &arg1)
{
    qDebug() << arg1;
    ((QPushButton *)ui->buttonBox->buttons().first())->setEnabled(false);
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    foreach (QString proFile, m_proList)
    {
        QDir dir(m_dir);
        QString display = dir.relativeFilePath(proFile);
        if (!arg1.isEmpty() && !display.contains(arg1))
            continue;
        ui->listWidget->addItem(display);
    }
    ui->listWidget->blockSignals(false);
}
