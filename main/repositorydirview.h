#ifndef REPOSITORYDIRVIEW_H
#define REPOSITORYDIRVIEW_H
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
class RepositoryDirView : public QListView
{
    Q_OBJECT
public:
    explicit RepositoryDirView(QWidget *parent = nullptr);
    QString rootPath();
    QFileSystemModel &model();
    void loadDir(QString rootPath);
    void upDir();
private:
    QString m_rootPath;
    QFileSystemModel m_model;
signals:
    void rootPathChanged(QString rootPath);
    void fileDoubleClicked(QString path);
private slots:
    void on_doubleClicked(const QModelIndex &index);
};
#endif // REPOSITORYDIRVIEW_H
