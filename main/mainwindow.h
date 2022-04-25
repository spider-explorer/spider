#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "common.h"
#include "spidercore.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
//#include "eventeater.h"
QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
// EventEater m_eventEater;
    void reloadExplorer(QString topDir);
    void reloadNameList(bool force = false);
    QString selectedRepoName();
    QString selectedMsys2Name();
    void open_elvish(QString repo);
    void open_emacs(QString repoDir);
    void open_vscode(QString repoDir);
    void open_git_page(QString repo);
    void open_smartgit(QString repo);
    void open_git_gui(QString repo);
    void clone_repo();
    void remove_repo(QString repo);
    void refresh_repo(QString repo);
    void open_explorer(QString repo);
    void develop_with_qtcreator(QString proFile);
    void develop_with_android_studio(QString repoDir);
    void develop_with_idea(QString repoDir);
    void remove_msys2(QString name);
    void install_or_update_flutter();
    void install_msys2();
private slots:
// void showContextMenuForListWidget1(const QPoint& pos);
    void showContextMenuForListWidget1(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index);
    void showContextMenuForListWidget2(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index);
    void showContextMenuForListWidget3(QListWidget *listWidget, QMenu &contextMenu, QModelIndex index);
// void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void doubleClickedButtons(QListWidgetItem *item, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void on_btnCloneRepo_clicked();
    void on_btnResetSelection_clicked();
    void on_btnNyagos_clicked();
    void on_btnMsys2_clicked();
    void on_btnBash_clicked();
    void on_btnElvish_clicked();
    void on_btnFlutterInstall_clicked();
    void on_btnMsys2Install_clicked();
    void on_btnEnvVarManage_clicked();
    void on_btnScoopManage_clicked();
    void on_btnSettingsFolderOpen_clicked();
    void on_actionChrome_triggered();
    void on_actionFolderSize_triggered();
    void on_actionRapidee_triggered();
    void on_actionSQLiteStudio_triggered();
    void on_actionExit_triggered();
    void on_scoopSearch_clicked();
// void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_actionQtCreator_triggered();
    void on_listWidget_3_itemDoubleClicked(QListWidgetItem *item);
    void on_listWidget_3_itemSelectionChanged();
    void on_actionNotepad3_triggered();
    void on_btnUbuntuInstall_clicked();
    void on_btnUbuntuImport_clicked();
    void on_actionLibreOffice_triggered();
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_treeView_clicked(const QModelIndex &index);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_tabWidget_currentChanged(int index);
    void on_actionTest01_triggered();
private:
// SpiderCore& m_core;
    QTimer m_reload_timer;
    QString m_topDir;
    QFileSystemModel m_dirModel;
    QFileSystemModel m_fileModel;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
