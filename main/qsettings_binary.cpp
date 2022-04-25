#include "qsettings_binary.h"
#include "variantserializer.h"
#include <QtCore>
bool readSettingsBinary(QIODevice &device, QMap<QString, QVariant> &map)
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
                QVariant v2 = VariantSerializer().deserializeFromString(obj2[key2].toString());
                if (key == "[General]")
                {
                    map[key2] = v2;
                }
                else
                {
                    map[key + "/" + key2] = v2;
                }
            }
            // continue;
        }
        // QVariant v =
        // VariantSerializer().deserializeFromString(obj[key].toString());
        // map[key] = v;
    }
    return true;
}
bool writeSettingsBinary(QIODevice &device, const QMap<QString, QVariant> &map)
{
    QVariantMap map2;
    QStringList keys = map.keys();
    foreach (QString key, keys)
    {
        QString bin = VariantSerializer().serializeToString(map[key]);
        if (key.contains("/"))
        {
            QStringList groupKeys = key.split("/");
            if (groupKeys.size() == 2)
            {
                QVariantMap groupMap = map2[groupKeys[0]].toMap();
                groupMap[groupKeys[1]] = bin;
                map2[groupKeys[0]] = groupMap;
            }
        }
        else
        {
            QVariantMap groupMap = map2["[General]"].toMap();
            groupMap[key] = bin;
            map2["[General]"] = groupMap;
        }
    }
    QJsonDocument jdoc = QJsonDocument::fromVariant(map2);
    device.write(jdoc.toJson());
    return true;
}
