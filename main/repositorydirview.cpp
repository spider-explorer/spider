#include "repositorydirview.h"
RepositoryDirView::RepositoryDirView(QWidget *parent) : QListView(parent)
{
    connect(this, &RepositoryDirView::doubleClicked, this, &RepositoryDirView::on_doubleClicked);
}
QString RepositoryDirView::rootPath()
{
    return m_rootPath;
}
QFileSystemModel &RepositoryDirView::model()
{
    return m_model;
}
void RepositoryDirView::loadDir(QString rootPath)
{
    m_rootPath = rootPath;
    m_model.setRootPath(m_rootPath);
    this->setModel(&m_model);
    this->setRootIndex(m_model.index(m_rootPath));
    emit rootPathChanged(m_rootPath);
}
void RepositoryDirView::upDir()
{
    if (m_rootPath.isEmpty())
        return;
    QString up = QFileInfo(m_rootPath).absoluteDir().path();
    qDebug() << up << m_rootPath;
    if (up == m_rootPath)
    {
        up = "";
    }
    this->loadDir(up);
}
void RepositoryDirView::on_doubleClicked(const QModelIndex &index)
{
    qDebug() << m_model.filePath(index);
    QString path = m_model.filePath(index);
    if (QFileInfo(path).isDir())
    {
        this->loadDir(path);
    }
    else
    {
        emit fileDoubleClicked(path);
    }
}
