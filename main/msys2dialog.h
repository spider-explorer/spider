#ifndef MSYS2DIALOG_H
#define MSYS2DIALOG_H
#include <QDialog>
namespace Ui
{
class Msys2Dialog;
}
class Msys2Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Msys2Dialog(QWidget *parent = nullptr);
    ~Msys2Dialog();
    QString name() const;
private slots:
    void on_lineEdit_textChanged(const QString &arg1);
private:
    Ui::Msys2Dialog *ui;
};
#endif // MSYS2DIALOG_H
