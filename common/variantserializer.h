#ifndef VARIANTSERIALIZER_H
#define VARIANTSERIALIZER_H
#include <QtCore>
class VariantSerializer
{
public:
    VariantSerializer();
    QByteArray serialize(const QVariant &x);
    QVariant deserialize(const QByteArray &x);
    QString serializeToString(const QVariant &x);
    QVariant deserializeFromString(const QString &x);
    QByteArray serializeToBinary(const QVariant &x);
    QVariant deserializeFromBinary(const QByteArray &x);
};
#endif // VARIANTSERIALIZER_H
