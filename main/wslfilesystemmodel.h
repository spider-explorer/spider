#ifndef WSLFILESYSTEMMODEL_H
#define WSLFILESYSTEMMODEL_H
#include <QFileIconProvider>
#include <QtCore>
class WslFileSystemNode
{
public:
    WslFileSystemNode(QFileInfo *self, QFileInfo *parent, int row) : m_parent(parent), m_self(self), m_row(row)
    {
    }
    QFileInfo *m_parent;
    QFileInfo *m_self;
    int m_row;
};
class WslFileSystemDB
{
public:
    WslFileSystemDB()
    {
        m_db1 = new QMap<QString, WslFileSystemNode *>();
        m_db2 = new QMap<QFileInfo *, WslFileSystemNode *>();
    }
    WslFileSystemNode *find(QString path) const
    {
        path = path.replace("/", "\\");
        return (*m_db1)[path];
    }
    WslFileSystemNode *find(QFileInfo *info) const
    {
        return (*m_db2)[info];
    }
    WslFileSystemNode *insert(QString path, QFileInfo *parent, int row) const
    {
        path = path.replace("/", "\\");
        WslFileSystemNode *result = find(path);
        if (result != nullptr)
            return result;
        result = new WslFileSystemNode(new QFileInfo(path), parent, row);
        (*m_db1)[path] = result;
        (*m_db2)[result->m_self] = result;
        return result;
    }
    QMap<QString, WslFileSystemNode *> *m_db1;
    QMap<QFileInfo *, WslFileSystemNode *> *m_db2;
};
class WslFileSystemModel : public QAbstractItemModel
{
public:
    WslFileSystemModel();
public:
// QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
//
public:
    QFileInfo *info(const QModelIndex &index) const;
    static QList<QFileInfo *> childrenOf(const WslFileSystemModel *that, QFileInfo *parent);
public:
    QString m_rootPath = R"***(\\wsl.localhost\Ubuntu)***";
    QList<QFileInfo *> m_infoList;
    WslFileSystemDB *m_db;
    QFileIconProvider m_iconProvider;
};
#endif // WSLFILESYSTEMMODEL_H
