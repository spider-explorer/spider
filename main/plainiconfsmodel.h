#ifndef PLAINICONFSMODEL_H
#define PLAINICONFSMODEL_H
#include <QFileIconProvider>
#include <QFileSystemModel>
#include <QListWidgetItem>
#include <QtCore>
class PlainIconFSModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit PlainIconFSModel(QObject *parent = nullptr);
    ~PlainIconFSModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
// QMap<QString, QString> &extMap();
    void setListWidgetItemIcon(QListWidgetItem *item, QString path);
private:
    void AddLex(QString ext, QString iconId);
private:
    QMap<QString, QString> m_extMap;
};
#endif // PLAINICONFSMODEL_H
