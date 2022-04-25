#ifndef REPODIALOG_H
#define REPODIALOG_H
#include <QDialog>
namespace Ui
{
class RepoDialog;
}
class RepoDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RepoDialog(QWidget *parent = nullptr);
    ~RepoDialog();
// QString name() const;
    QString url() const;
private:
    QString m_url;
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
private:
    Ui::RepoDialog *ui;
};
#endif // REPODIALOG_H
