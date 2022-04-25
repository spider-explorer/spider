#ifndef SPIDERMAIN_H
#define SPIDERMAIN_H
#include "common.h"
#include <QMainWindow>
namespace Ui
{
class SpiderMain;
}
class SpiderMain : public QMainWindow
{
    Q_OBJECT
public:
    explicit SpiderMain(QWidget *parent = nullptr);
    ~SpiderMain();
    void closeEvent(QCloseEvent *event);
public slots:
    void on__signal_entry2();
private slots:
    void on_btnNyagos_clicked();
    void on_btnBash_clicked();
    void on_actionTest01_triggered();
    void on_btnExplorer_clicked();
    void on_actionChrome_triggered();
    void on_actionFolderSize_triggered();
    void on_actionRapidEE_triggered();
    void on_actionSQLiteStudio_triggered();
    void on_actionNotepad3_triggered();
    void on_actionLibreOffice_triggered();
    void on_actionQtCreator_triggered();
    void on_actionQOwnNotes_triggered();
    void on_actionTest02_triggered();
    void on_actionManageScoop_triggered();
    void on_actionManageEnvVars_triggered();
    void on_actionLazarus_triggered();
    void on_actionAirExplorer_triggered();
    void on_actionEverything_triggered();
    void on_actionJoinMP4Files_triggered();
private:
    Ui::SpiderMain *ui;
};
#endif // SPIDERMAIN_H
