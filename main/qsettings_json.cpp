#include "qsettings_json.h"
#include <QtCore>
bool readSettingsJson(QIODevice &device, QMap<QString, QVariant> &map)
{
    QByteArray json = device.readAll();
    QJsonParseError error;
    QJsonDocument jdoc = QJsonDocument::fromJson(json, &error);
    if (error.error != QJsonParseError::NoError)
    {
        return false;
    }
    QJsonObject obj = jdoc.object();
    QStringList keys = obj.keys();
    map.clear();
    foreach (QString key, keys)
    {
        if (obj[key].isObject())
        {
            QJsonObject obj2 = obj[key].toObject();
            QStringList keys2 = obj2.keys();
            foreach (QString key2, keys2)
            {
                if (key == "[General]")
                {
                    map[key2] = obj2[key2].toVariant();
                }
                else
                {
                    map[key + "/" + key2] = obj2[key2].toVariant();
                }
            }
            // continue;
        }
        // map[key] = obj[key].toVariant();
    }
    return true;
}
bool writeSettingsJson(QIODevice &device, const QMap<QString, QVariant> &map)
{
    QVariantMap map2;
    QStringList keys = map.keys();
    foreach (QString key, keys)
    {
        if (key.contains("/"))
        {
            QStringList groupKeys = key.split("/");
            if (groupKeys.size() == 2)
            {
                QVariantMap groupMap = map2[groupKeys[0]].toMap();
                groupMap[groupKeys[1]] = map[key];
                map2[groupKeys[0]] = groupMap;
            }
        }
        else
        {
            QVariantMap groupMap = map2["[General]"].toMap();
            groupMap[key] = map[key];
            map2["[General]"] = groupMap;
        }
    }
    QJsonDocument jdoc = QJsonDocument::fromVariant(map2);
    device.write(jdoc.toJson());
    return true;
}
