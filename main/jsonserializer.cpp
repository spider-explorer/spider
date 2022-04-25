#include "jsonserializer.h"
JsonSerializer::JsonSerializer()
{
}
QByteArray JsonSerializer::serialize(const QVariant &x)
{
    QJsonDocument jdoc = QJsonDocument::fromVariant(x);
    return jdoc.toJson();
}
QByteArray JsonSerializer::serialize(const QJsonDocument &x)
{
    return x.toJson();
}
QByteArray JsonSerializer::serialize(const QJsonArray &x)
{
    QJsonDocument jdoc(x);
    return jdoc.toJson();
}
QByteArray JsonSerializer::serialize(const QJsonObject &x)
{
    QJsonDocument jdoc(x);
    return jdoc.toJson();
}
QJsonDocument JsonSerializer::deserialize(const QByteArray &x)
{
    QJsonDocument jdoc = QJsonDocument::fromJson(x);
    return jdoc;
}
