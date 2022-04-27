#include "variantserializer.h"
static QString variantToString(const QVariant &v)
{
    QJsonDocument jdoc = QJsonDocument::fromVariant(v);
    QString json = QString::fromUtf8(jdoc.toJson(QJsonDocument::Compact));
    if (!json.isEmpty())
    {
        return json;
    }
    QVariantList vl = {v};
    jdoc = QJsonDocument::fromVariant(vl);
    json = QString::fromUtf8(jdoc.toJson(QJsonDocument::Compact));
    json = json.mid(1, json.length() - 2); // remove [ & ]
    return json;
}
VariantSerializer::VariantSerializer()
{
}
QByteArray VariantSerializer::serialize(const QVariant &x)
{
    return this->serializeToBinary(x).toBase64();
}
QVariant VariantSerializer::deserialize(const QByteArray &x)
{
    return this->deserializeFromBinary(QByteArray::fromBase64(x));
}
QString VariantSerializer::serializeToString(const QVariant &x)
{
    QString s = VariantSerializer().serialize(x);
    // s += " " + variantToString(x);
    return s;
}
QVariant VariantSerializer::deserializeFromString(const QString &x)
{
    QString s = x;
    // s = s.split(" ").at(0);
    return VariantSerializer().deserialize(s.toLatin1());
}
QByteArray VariantSerializer::serializeToBinary(const QVariant &x)
{
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out << x;
    return message;
}
QVariant VariantSerializer::deserializeFromBinary(const QByteArray &x)
{
    QByteArray message = x;
    QVariant v;
    QDataStream in(&message, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_1);
    in >> v;
    return v;
}
