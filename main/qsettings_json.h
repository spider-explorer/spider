#ifndef QSETTINGS_JSON_H
#define QSETTINGS_JSON_H
#include <QtCore>
bool readSettingsJson(QIODevice &device, QMap<QString, QVariant> &map);
bool writeSettingsJson(QIODevice &device, const QMap<QString, QVariant> &map);
static const QSettings::Format JsonFormat = QSettings::registerFormat("json", &readSettingsJson, &writeSettingsJson);
#endif // QSETTINGS_JSON_H
