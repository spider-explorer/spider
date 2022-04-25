#ifndef ENVVARDIALOG_H
#define ENVVARDIALOG_H
#include <QDialog>
#include <QtCore>
namespace Ui
{
class EnvVarDialog;
}
class EnvVarDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EnvVarDialog(QMap<QString, QString> &env, QString repo, QWidget *parent = nullptr);
    ~EnvVarDialog();
private:
    QMap<QString, QString> m_env;
    QString m_repo;
// QString m_iniFileName;
    void reloadVariables();
    void setVariable(QString name, QString value);
private slots:
    void showContextMenuForListWidget1(const QPoint &pos);
    void on_btnClose_clicked();
    void on_btnAddVar_clicked();
    void on_listWidget_currentTextChanged(const QString &currentText);
    void on_btnTransferToPC_clicked();
private:
    Ui::EnvVarDialog *ui;
};
#endif // ENVVARDIALOG_H
