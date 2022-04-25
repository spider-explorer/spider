#ifndef REPOSITORYDIRDIALOG_H
#define REPOSITORYDIRDIALOG_H
#include "spidercore.h"
#include <QDialog>
#include <QtCore>
namespace Ui
{
class RepositoryDirDialog;
}
class RepositoryDirDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RepositoryDirDialog(QString rootPath, QWidget *parent = nullptr);
    ~RepositoryDirDialog();
private slots:
    void on_btnUpDir_clicked();
    void on_btnNyagos_clicked();
    void on_btnBash_clicked();
    void on_btnOffice_clicked();
private:
// QMap<QString, QString> m_env;
// SpiderCore& m_core;
    QString m_rootPath;
private:
    Ui::RepositoryDirDialog *ui;
};
#endif // REPOSITORYDIRDIALOG_H
