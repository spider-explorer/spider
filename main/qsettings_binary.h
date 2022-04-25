#ifndef QSETTINGS_BINARY_H
#define QSETTINGS_BINARY_H
#include <QtCore>
bool readSettingsBinary(QIODevice &device, QMap<QString, QVariant> &map);
bool writeSettingsBinary(QIODevice &device, const QMap<QString, QVariant> &map);
static const QSettings::Format BinaryFormat =
    QSettings::registerFormat("bin.json", &readSettingsBinary, &writeSettingsBinary);
#endif // QSETTINGS_BINARY_H
