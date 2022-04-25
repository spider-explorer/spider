#ifndef COMMON_H
#define COMMON_H
#include "binarysettings.h"
#include <QtCore>
#include <QtGui>
#include <QtWidgets>
static QString var(const QVariant &v, bool pretty = false)
{
    QJsonDocument jdoc = QJsonDocument::fromVariant(v);
    QString json = QString::fromUtf8(jdoc.toJson(pretty ? QJsonDocument::Indented : QJsonDocument::Compact));
    if (!json.isEmpty())
    {
        return json.trimmed();
    }
    QVariantList vl = {v};
    jdoc = QJsonDocument::fromVariant(vl);
    json = QString::fromUtf8(jdoc.toJson(QJsonDocument::Compact));
    json = json.mid(1, json.length() - 2); // remove [ & ]
    return json;
}
#if 0x0
class MySettings : public BinarySettings
{
public:
#if 0x0
    MySettings() : BinarySettings(JsonFormat, QSettings::UserScope, "javacommons", "spider")
    {
    }
#else
    MySettings() : BinarySettings(IniFormat, QSettings::UserScope, "javacommons", "spider")
    {
    }
#endif
    const QString SELECTED_REPO_NAME = "selected/repoName";
    const QString SELECTED_MSYS2_NAME = "selected/msys2Name";
};
#endif
class SpiderSettings
{
public:
    SpiderSettings(QMap<QString, QString> env, QString repo)
    {
        QString repoDir = env["docs"] + "/.repo";
        QString fileName = repoDir;
        if (!repo.isEmpty())
        {
            fileName += "/" + repo;
        }
        fileName += "/.spider.ini";
        m_settings = new BinarySettings(fileName, QSettings::IniFormat);
    }
    virtual ~SpiderSettings()
    {
        delete m_settings;
    }
    QSettings &settings()
    {
        return *(this->m_settings);
    }
private:
    BinarySettings *m_settings;
};
static inline QString np(QString x)
{
    return x.replace("/", "\\");
}
static inline void updateListWidgetContent(QListWidget *lw, QStringList *newContent, const QString &selectedName,
        bool force = false)
{
    QStringList currRepoList;
    QMap<QString, QBrush> currColorMap;
    QString currentName;
    for (int i = 0; i < lw->count(); i++)
    {
        if (lw->item(i)->isSelected())
            currentName = lw->item(i)->text();
        currColorMap[lw->item(i)->text()] = lw->item(i)->foreground();
        currRepoList.append(lw->item(i)->text());
    }
    if (!force && currRepoList == *newContent)
        return;
    lw->blockSignals(true);
    lw->clear();
    lw->blockSignals(false);
    for (int i = 0; i < newContent->count(); i++)
    {
        QString dir = newContent->at(i);
        qDebug() << dir;
        lw->addItem(dir);
        lw->item(lw->count() - 1)->setForeground(currColorMap[dir]);
        if (dir == currentName)
        {
            lw->item(lw->count() - 1)->setSelected(true);
        }
        if (dir == selectedName)
        {
            lw->item(lw->count() - 1)->setIcon(QApplication::style()->standardPixmap(QStyle::SP_ArrowForward));
        }
        else
        {
            lw->item(lw->count() - 1)->setIcon(QApplication::style()->standardPixmap(QStyle::SP_DirIcon));
        }
    }
}
static bool isVaridFolderName(QString x)
{
    QRegExp rx("^[a-z0-9._-]+$", Qt::CaseSensitivity::CaseInsensitive);
    return rx.exactMatch(x);
}
#endif // COMMON_H
