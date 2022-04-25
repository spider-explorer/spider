#ifndef SCOOPSEARCHDIALOG_H
#define SCOOPSEARCHDIALOG_H
//#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
namespace Ui
{
class ScoopSearchDialog;
}
class ScoopSearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScoopSearchDialog(QMap<QString, QString> env, QWidget *parent = nullptr);
    ~ScoopSearchDialog();
    QStringList nameList();
    QMap<QString, QVariantMap> statusMap();
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
    void on_listWidget_itemChanged(QListWidgetItem *item);
    void on_listWidget_itemSelectionChanged();
private:
    QMap<QString, QString> m_env;
    QVariantMap m_map;
private:
    Ui::ScoopSearchDialog *ui;
};
#endif // SCOOPSEARCHDIALOG_H
