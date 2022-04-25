#include "jsonsettings.h"
#include "jinstaller.h"
#include "variantserializer.h"
#include "jnetwork.h"
JsonSettings::JsonSettings(const QString &application) : m_type(JSET_APPL)
{
    QString homeLocation = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString settingsFolder = homeLocation + "/.config/spider";
    QDir(settingsFolder).mkpath(".");
    m_jsonPath = settingsFolder + "/" + application + ".json";
    QFile jsonFile(m_jsonPath);
    if (jsonFile.open(QIODevice::ReadOnly))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonFile.readAll());
        m_varMap = jsonDoc.object().toVariantMap();
    }
}
JsonSettings::JsonSettings(const QUrl &url) : m_type(JSET_URL)
{
    JNetworkManager nm;
    //m_jsonPath = url.toString(QUrl::None);
    //QSplashScreen splash;
    QString json = nm.getBatchAsText(url);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8());
    m_varMap = jsonDoc.object().toVariantMap();
}
JsonSettings::~JsonSettings()
{
    this->sync();
}
void JsonSettings::sync()
{
    if (m_type == JSET_URL)
        return;
    QFile jsonFile(m_jsonPath);
    if (jsonFile.open(QIODevice::WriteOnly))
    {
        QJsonDocument jsonDoc = QJsonDocument::fromVariant(m_varMap);
        jsonFile.write(jsonDoc.toJson());
    }
}
QVariantMap &JsonSettings::object()
{
    return m_varMap;
}
void JsonSettings::beginGroup(const QString &prefix)
{
    m_prefix = prefix;
}
void JsonSettings::endGroup()
{
    m_prefix = "";
}
QString JsonSettings::group() const
{
    return m_prefix;
}
QString JsonSettings::fileName()
{
    return m_jsonPath;
}
void JsonSettings::setValue(const QString &key, const QVariant &value)
{
    QStringList keys = m_prefix.isEmpty() ? key.split("/") : (m_prefix + "/" + key).split("/");
    QStack<QVariant> varStack;
    varStack.push(m_varMap);
    for (int i = 0; i < keys.size() - 1; i++)
    {
        varStack.push(varStack.top().toMap().value(keys[i]).toMap());
    }
    varStack.push(value);
    int i = keys.size();
    QVariant val = varStack.pop();
    while (varStack.size() >= 2)
    {
        QVariantMap parent = varStack.pop().toMap();
        parent.insert(keys.at(--i), val);
        val = parent;
    }
    m_varMap.insert(keys[0], val);
}
QVariant JsonSettings::value(const QString &key, const QVariant &defaultValue) const
{
    QStringList keys = m_prefix.isEmpty() ? key.split("/") : (m_prefix + "/" + key).split("/");
    QStack<QVariant> varStack;
    varStack.push(m_varMap);
    for (int i = 0; i < keys.size() - 1; i++)
    {
        varStack.push(varStack.top().toMap().value(keys[i]).toMap());
    }
    return varStack.top().toMap().value(keys.last(), defaultValue);
}
void JsonSettings::remove(const QString &key)
{
    QStringList keys = m_prefix.isEmpty() ? key.split("/") : (m_prefix + "/" + key).split("/");
    if (keys.size() == 1)
    {
        m_varMap.remove(keys[0]);
        return;
    }
    QStack<QVariant> varStack;
    varStack.push(m_varMap);
    for (int i = 0; i < keys.size() - 1; i++)
    {
        varStack.push(varStack.top().toMap().value(keys[i]).toMap());
    }
    int i = keys.size();
    QVariant val = varStack.pop();
    QVariantMap map = val.toMap();
    map.remove(keys.at(--i));
    val = map;
    while (varStack.size() >= 2)
    {
        QVariantMap parent = varStack.pop().toMap();
        parent.insert(keys.at(--i), val);
        val = parent;
    }
    m_varMap.insert(keys[0], val);
}
bool JsonSettings::contains(const QString &key) const
{
    QStringList keys = m_prefix.isEmpty() ? key.split("/") : (m_prefix + "/" + key).split("/");
    QStack<QVariant> varStack;
    varStack.push(m_varMap);
    for (int i = 0; i < keys.size() - 1; i++)
    {
        varStack.push(varStack.top().toMap().value(keys[i]).toMap());
    }
    return varStack.top().toMap().contains(keys.last());
}
void JsonSettings::setBinary(const QString &key, const QVariant &value)
{
    this->setValue(key, VariantSerializer().serializeToString(value));
}
QVariant JsonSettings::binary(const QString &key, const QVariant &defaultValue) const
{
    return VariantSerializer().deserializeFromString(this->value(key).toString());
}
