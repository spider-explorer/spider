#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H
#include <QtCore>
class JsonSerializer
{
public:
    JsonSerializer();
    QByteArray serialize(const QVariant &x);
    QByteArray serialize(const QJsonDocument &x);
    QByteArray serialize(const QJsonArray &x);
    QByteArray serialize(const QJsonObject &x);
    QJsonDocument deserialize(const QByteArray &x);
};
#endif // JSONSERIALIZER_H
