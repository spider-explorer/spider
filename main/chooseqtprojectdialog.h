#ifndef CHOOSEQTPROJECTDIALOG_H
#define CHOOSEQTPROJECTDIALOG_H
//#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
namespace Ui
{
class ChooseQtProjectDialog;
}
class ChooseQtProjectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseQtProjectDialog(QString dir, QStringList proList, QWidget *parent = nullptr);
    ~ChooseQtProjectDialog();
    QString proFile() const;
private:
    QStringList m_proList;
    QString m_dir;
    QString m_proFile;
private slots:
    void on_listWidget_itemSelectionChanged();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_lineEdit_textChanged(const QString &arg1);
private:
    Ui::ChooseQtProjectDialog *ui;
};
#endif // CHOOSEQTPROJECTDIALOG_H
