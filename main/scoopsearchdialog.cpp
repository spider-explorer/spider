#include "scoopsearchdialog.h"
#include "ui_scoopsearchdialog.h"
ScoopSearchDialog::ScoopSearchDialog(QMap<QString, QString> env, QWidget *parent)
    : QDialog(parent), m_env(env), ui(new Ui::ScoopSearchDialog)
{
    ui->setupUi(this);
    ui->label->setWordWrap(true);
    ui->label->setText("");
    ui->textBrowser->setWordWrapMode(QTextOption::NoWrap);
    qDebug() << m_env["prof"];
    QString jsonFileName = m_env["dir"] + "/scoop-bucket-map.json";
    QFile jsonFile(jsonFileName);
    if (jsonFile.open(QIODevice::ReadOnly))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
        m_map = jsonDoc.toVariant().toMap();
        // qDebug() << m_map;
        QStringList keys = m_map.keys();
        foreach (QString key, keys)
        {
            QString appPath = m_env["prof"] + "/scoop/apps/" + key;
            bool installed = QFileInfo(appPath).exists();
            QVariantMap appMap = m_map[key].toMap();
            // qDebug() << appMap;
            appMap["installed"] = installed;
            m_map[key] = appMap;
        }
    }
    this->on_lineEdit_textChanged("");
}
ScoopSearchDialog::~ScoopSearchDialog()
{
    delete ui;
}
QStringList ScoopSearchDialog::nameList()
{
    QStringList keys = m_map.keys();
    keys.sort();
    return keys;
}
QMap<QString, QVariantMap> ScoopSearchDialog::statusMap()
{
    QMap<QString, QVariantMap> result;
    QStringList keys = m_map.keys();
    foreach (QString key, keys)
    {
        result[key] = m_map[key].toMap();
    }
    return result;
}
void ScoopSearchDialog::on_lineEdit_textChanged(const QString &arg1)
{
    ui->label->setText("");
    ui->textBrowser->setText("");
    // qDebug() << arg1;
    QStringList keys = m_map.keys();
    keys.sort();
    ui->listWidget->blockSignals(true);
    ui->listWidget->clear();
    foreach (QString key, keys)
    {
        QString bucket = m_map[key].toMap()["bucket"].toString();
        QString desc = m_map[key].toMap()["description"].toString();
        if (!arg1.isEmpty() && !key.toLower().contains(arg1.toLower()) && !desc.toLower().contains(arg1.toLower()))
            continue;
        bool installed = m_map[key].toMap()["installed"].toBool();
        if (arg1.isEmpty() && !installed)
            continue;
        QListWidgetItem *item = new QListWidgetItem(QString("%1 (%2)").arg(key).arg(bucket), ui->listWidget);
        item->setData(Qt::UserRole, key);
        item->setToolTip(QString("[%1] %2").arg(key).arg(desc));
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        if (installed)
        {
            item->setCheckState(Qt::Checked);
        }
        else
        {
            item->setCheckState(Qt::Unchecked);
        }
    }
    ui->listWidget->blockSignals(false);
}
void ScoopSearchDialog::on_listWidget_itemChanged(QListWidgetItem *item)
{
    QString key = item->data(Qt::UserRole).toString();
    bool installed = false;
    if (item->checkState() == Qt::Checked)
    {
        qDebug() << item->text() << "[on]";
        installed = true;
    }
    else if (item->checkState() == Qt::Unchecked)
    {
        qDebug() << item->text() << "[off]";
        installed = false;
    }
    QVariantMap appMap = m_map[key].toMap();
    appMap["installed"] = installed;
    m_map[key] = appMap;
}
void ScoopSearchDialog::on_listWidget_itemSelectionChanged()
{
    QListWidgetItem *current = ui->listWidget->currentItem();
    if (current == nullptr)
    {
        ui->label->setText("");
        return;
    }
    QString key = current->data(Qt::UserRole).toString();
    qDebug() << key;
    ui->label->setText(QString("[%1]\n%2").arg(key).arg(m_map[key].toMap()["description"].toString()));
    QString jsonPath =
        m_env["prof"] + "/scoop/buckets/" + m_map[key].toMap()["bucket"].toString() + "/bucket/" + key + ".json";
    qDebug() << QFileInfo(jsonPath).exists();
    QFile jsonFile(jsonPath);
    if (jsonFile.open(QIODevice::ReadOnly))
    {
        ui->textBrowser->setText(QString::fromUtf8(jsonFile.readAll()));
    }
}
