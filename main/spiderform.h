#ifndef SPIDERFORM_H
#define SPIDERFORM_H
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
namespace Ui
{
class SpiderForm;
}
class SpiderForm : public QWidget
{
    Q_OBJECT
public:
    explicit SpiderForm(QWidget *parent = nullptr);
    ~SpiderForm();
    void reloadNameList(bool force = false);
    void setCurrentFolder(QString path);
private:
    void checkGitStatus(QString path, bool full = false);
    void checkGitStatusAll(bool full = false);
private slots:
    void on_splitter_1_splitterMoved(int pos, int index);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_btnCloneRepo_clicked();
    void on_btnResetSelection_clicked();
    void on_btnRepoStatus_clicked();
    void on_listWidget_2_itemDoubleClicked(QListWidgetItem *item);
    void on_btnMsys2Install_clicked();
signals:
    void signal_repoRequestedContextMenu(QMenu &contextMenu, QString path);
    void signal_repoClicked(QString path);
    void signal_repoDoubleClicked(QString path, Qt::MouseButton button, Qt::KeyboardModifiers modifiers);
    void signal_msys2RequestedContextMenu(QMenu &contextMenu, QString path);
    void signal_msys2DoubleClicked(QString path);
private:
// QFileSystemWatcher m_watcher;
    QTimer m_reload_timer;
    QTimer m_watch_timer;
// QTimer m_repo_check_timer;
    QMutex m_mutex;
private:
    Ui::SpiderForm *ui;
};
#endif // SPIDERFORM_H
