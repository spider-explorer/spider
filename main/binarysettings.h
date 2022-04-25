#ifndef BINARYSETTINGS_H
#define BINARYSETTINGS_H
#include "qsettings_binary.h"
#include "qsettings_json.h"
#include <QtCore>
class BinarySettings : public QSettings
{
public:
    explicit BinarySettings(const QString &organization, const QString &application = QString(),
                            QObject *parent = nullptr)
        : QSettings(organization, application, parent)
    {
    }
    BinarySettings(Scope scope, const QString &organization, const QString &application = QString(),
                   QObject *parent = nullptr)
        : QSettings(scope, organization, application, parent)
    {
    }
    BinarySettings(Format format, Scope scope, const QString &organization, const QString &application = QString(),
                   QObject *parent = nullptr)
        : QSettings(format, scope, organization, application, parent)
    {
    }
    BinarySettings(const QString &fileName, Format format, QObject *parent = nullptr)
        : QSettings(fileName, format, parent)
    {
    }
    explicit BinarySettings(QObject *parent = nullptr) : QSettings(parent)
    {
    }
    explicit BinarySettings(Scope scope, QObject *parent = nullptr) : QSettings(scope, parent)
    {
    }
    void setBinary(const QString &key, const QVariant &value);
    QVariant binary(const QString &key, const QVariant &defaultValue = QVariant()) const;
};
#endif // BINARYSETTINGS_H
