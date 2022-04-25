#ifndef EXPLORERFORM_H
#define EXPLORERFORM_H
#include "plainiconfsmodel.h"
#include <QFileSystemModel>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
namespace Ui
{
class ExplorerForm;
}
class ExplorerForm : public QWidget
{
    Q_OBJECT
public:
    explicit ExplorerForm(QWidget *parent = nullptr);
    ~ExplorerForm();
    void reloadFileList(bool force = true);
    void setRootDir(QString path);
    QString currentDir();
    void setCurrentDir(QString path, bool emitSignal = false);
    QString saveState();
    void restoreState(QString s);
    QToolBar *topToolbar();
private:
    QString m_topDir;
    QString m_currDir;
#if 0x0
    QFileSystemModel m_dirModel;
    QFileSystemModel m_fileModel;
#else
    PlainIconFSModel m_dirModel;
    PlainIconFSModel m_fileModel;
#endif
private slots:
    void on_treeView_clicked(const QModelIndex &index);
    void on_listView_doubleClicked(const QModelIndex &index);
    void on_lineEdit_textChanged(const QString &arg1);
    void on_splitter_1_splitterMoved(int pos, int index);
    void on_splitter_2_splitterMoved(int pos, int index);
    void on_btnSearchClear_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on__searchFinished(qint64 threadId, QString topDir, QString rxString, QStringList pathList);
signals:
    void signal_folderRequestedContextMenu(QMenu &contextMenu, QString path);
    void signal_fileDoubleClicked(QString path);
    void signal_folderChanged(QString path);
    void signal_fileRequestedContextMenu(QMenu &contextMenu, QString path);
private:
    QString lastDoubleClickedFile;
    QDateTime lastDoubleClickedFileTime;
    QToolBar *m_topToolbar = nullptr;
private:
    Ui::ExplorerForm *ui;
};
#endif // EXPLORERFORM_H
