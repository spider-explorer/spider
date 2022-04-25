#include "scoopsoftwaredialog.h"
#include "scoopsoftwarelist.h"
#include "ui_scoopsoftwaredialog.h"
ScoopSoftwareDialog::ScoopSoftwareDialog(QMap<QString, QString> env, QWidget *parent)
    : QDialog(parent), m_env(env), ui(new Ui::ScoopSoftwareDialog)
{
    ui->setupUi(this);
    ui->label->setText("インストールしたいソフトウェアにチェックを入れてください");
    ui->listWidget->blockSignals(true);
    ScoopSoftwareList swList(m_env, m_env["dir"] + "/scoop-sw-list.ini");
    m_nameList = swList.nameList();
    m_statusMap = swList.statusMap();
    // qDebug() << m_statusMap;
    foreach (QString key, m_nameList)
    {
        QListWidgetItem *item = new QListWidgetItem(key, ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if (m_statusMap[key]["installed"].toBool())
        {
            item->setCheckState(Qt::Checked);
        }
        else
        {
            item->setCheckState(Qt::Unchecked);
        }
        ui->listWidget->addItem(item);
    }
    ui->listWidget->blockSignals(false);
    //
    QFile scoopList(m_env["dir"] + "/scoop-list.txt");
    if (scoopList.open(QIODevice::ReadOnly))
    {
        QByteArray bytes = scoopList.readAll();
        // qDebug().noquote() << bytes;
        QString text = QString::fromLatin1(bytes);
        qDebug().noquote() << text;
        QString line;
        QString pkg;
        QTextStream stream(&text);
        while (stream.readLineInto(&line))
        {
            // qDebug() << line;
            if (line.startsWith("'"))
            {
                pkg = line.replace("'", "");
                // qDebug() << pkg;
                continue;
            }
            qDebug() << line << pkg;
        }
        stream.seek(0);
    }
}
ScoopSoftwareDialog::~ScoopSoftwareDialog()
{
    delete ui;
}
QStringList ScoopSoftwareDialog::nameList()
{
    return m_nameList;
}
QMap<QString, QVariantMap> ScoopSoftwareDialog::statusMap()
{
    return m_statusMap;
}
void ScoopSoftwareDialog::on_listWidget_itemChanged(QListWidgetItem *item)
{
    if (item->checkState() == Qt::Checked)
    {
        qDebug() << item->text() << "[on]";
        m_statusMap[item->text()]["installed"] = true;
    }
    else if (item->checkState() == Qt::Unchecked)
    {
        qDebug() << item->text() << "[off]";
        m_statusMap[item->text()]["installed"] = false;
    }
}
