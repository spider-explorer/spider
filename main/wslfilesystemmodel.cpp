#include "wslfilesystemmodel.h"
WslFileSystemModel::WslFileSystemModel()
{
    qDebug() << "WslFileSystemModel::WslFileSystemModel()";
    m_db = new WslFileSystemDB();
    qDebug() << "WslFileSystemModel::WslFileSystemModel(end)";
}
QModelIndex WslFileSystemModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug() << "WslFileSystemModel::index()";
    if (!parent.isValid())
    {
        if (row == 0 && column == 0)
        {
            return createIndex(0, 0, (quintptr)m_db->insert(m_rootPath, nullptr, 0)->m_self);
        }
        return QModelIndex();
    }
    if (column != 0 || parent.column() != 0)
    {
        return QModelIndex();
    }
    QList<QFileInfo *> children = childrenOf(this, info(parent));
    if (row < children.size())
    {
        return createIndex(row, 0, (quintptr)children.at(row));
    }
    return QModelIndex();
}
QModelIndex WslFileSystemModel::parent(const QModelIndex &child) const
{
    qDebug() << "WslFileSystemModel::parent()";
    if (child.isValid())
    {
#if 0x1
        QFileInfo *self = info(child);
        if (self->absoluteFilePath().replace("/", "\\") != m_rootPath)
        {
            WslFileSystemNode *found = m_db->find(self);
            if (found != nullptr)
            {
                return createIndex(found->m_row, 0, (quintptr)found->m_parent);
            }
        }
#endif
    }
    return QModelIndex();
}
int WslFileSystemModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "WslFileSystemModel::rowCount()";
    return parent.isValid() ? childrenOf(this, info(parent)).size() : 1;
}
int WslFileSystemModel::columnCount(const QModelIndex &parent) const
{
    qDebug() << "WslFileSystemModel::columnCount()";
    return 1;
}
QVariant WslFileSystemModel::data(const QModelIndex &index, int role) const
{
    qDebug() << "WslFileSystemModel::data()";
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DecorationRole)
    {
        if (info(index)->isDir())
        {
            return m_iconProvider.icon(QFileIconProvider::Folder);
        }
        else
        {
            return m_iconProvider.icon(QFileIconProvider::File);
        }
    }
    else if (role != Qt::DisplayRole)
        return QVariant();
    QString result = info(index)->fileName();
    qDebug() << "data():" << result;
    return result;
}
QVariant WslFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    qDebug() << "WslFileSystemModel::headerData()";
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();
    return QString("Name");
}
QFileInfo *WslFileSystemModel::info(const QModelIndex &index) const
{
    qDebug() << "WslFileSystemModel::info()";
    return static_cast<QFileInfo *>(index.internalPointer());
}
QList<QFileInfo *> WslFileSystemModel::childrenOf(const WslFileSystemModel *that, QFileInfo *parent)
{
    qDebug() << "WslFileSystemModel::childrenOf()" << (void *)parent;
    qDebug() << "WslFileSystemModel::childrenOf(): " << parent->absoluteFilePath();
    QList<QFileInfo *> result;
    QDir::Filters filters = QDir::Dirs | QDir::Files;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    QDirIterator it(parent->absoluteFilePath().replace("/", "\\"), filters, flags);
    QStringList debugList;
    int row = 0;
    while (it.hasNext())
    {
        QString path = it.next();
        if (path.endsWith("/.") || path.endsWith("/..") || path.endsWith("\\.") || path.endsWith("\\.."))
            continue;
        path = path.replace("/", "\\");
        // qDebug() << "path:" << path;
        result.append(that->m_db->insert(path, parent, row++)->m_self);
        debugList.append(path);
    }
    // qDebug() << debugList;
    return result;
}
