#include "binarysettings.h"
#include "qsettings_binary.h"
#include "qsettings_json.h"
#include "variantserializer.h"
void BinarySettings::setBinary(const QString &key, const QVariant &value)
{
    if (this->format() == BinaryFormat)
    {
        this->setValue(key, value);
    }
    else
    {
        this->setValue(key, VariantSerializer().serializeToString(value));
    }
    this->setValue(key, VariantSerializer().serializeToString(value));
}
QVariant BinarySettings::binary(const QString &key, const QVariant &defaultValue) const
{
    if (!this->contains(key))
    {
        return defaultValue;
    }
    if (this->format() == BinaryFormat)
    {
        return this->value(key);
    }
    else
    {
        return VariantSerializer().deserializeFromString(this->value(key).toString());
    }
}
