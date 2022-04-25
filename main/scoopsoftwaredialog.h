#ifndef SCOOPSOFTWAREDIALOG_H
#define SCOOPSOFTWAREDIALOG_H
//#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
namespace Ui
{
class ScoopSoftwareDialog;
}
class ScoopSoftwareDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ScoopSoftwareDialog(QMap<QString, QString> env, QWidget *parent = nullptr);
    ~ScoopSoftwareDialog();
    QStringList nameList();
    QMap<QString, QVariantMap> statusMap();
private:
    QStringList m_nameList;
    QMap<QString, QVariantMap> m_statusMap;
private slots:
    void on_listWidget_itemChanged(QListWidgetItem *item);
private:
    QMap<QString, QString> m_env;
private:
    Ui::ScoopSoftwareDialog *ui;
};
#endif // SCOOPSOFTWAREDIALOG_H
