#ifndef TEST01DIALOG_H
#define TEST01DIALOG_H
#include "wslfilesystemmodel.h"
#include <QDialog>
namespace Ui
{
class Test01Dialog;
}
class Test01Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Test01Dialog(QWidget *parent = nullptr);
    ~Test01Dialog();
private:
    WslFileSystemModel m_model;
private:
    Ui::Test01Dialog *ui;
};
#endif // TEST01DIALOG_H
