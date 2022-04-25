#ifndef JSONSETTINGS_H
#define JSONSETTINGS_H
#include <QtCore>
enum JsonSettingsType
{
    JSET_APPL,
    JSET_FILE,
    JSET_URL
};
class JsonSettings
{
public:
    explicit JsonSettings(const QString &application);
    explicit JsonSettings(const QUrl &url);
    virtual ~JsonSettings();
    QString fileName();
    void sync();
    QVariantMap &object();
//
    void beginGroup(const QString &prefix);
    void endGroup();
    QString group() const;
//
    void setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
//
    void remove(const QString &key);
    bool contains(const QString &key) const;
//
    void setBinary(const QString &key, const QVariant &value);
    QVariant binary(const QString &key, const QVariant &defaultValue = QVariant()) const;
private:
    JsonSettingsType m_type;
    QString m_jsonPath;
    QVariantMap m_varMap;
    QString m_prefix;
};
#endif // JSONSETTINGS_H
