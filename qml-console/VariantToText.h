#ifndef VARIANTTOTEXT_H
#define VARIANTTOTEXT_H
#include <QtCore>
#include "EscapeString.h"
static inline QString variantToText(const QVariant &x)
{
    switch(x.type())
    {
    case QVariant::Map:
    {
        QVariantMap map = x.toMap();
        QStringList keys = map.keys();
        QString result = "{";
        for(int i=0; i<keys.size(); i++)
        {
            if(i>0) result += ", ";
            QString key = keys[i];
            result += variantToText(key);
            result += ": ";
            result += variantToText(map[key]);
        }
        result += "}";
        return result;
    }
    case QVariant::String:
    {
        QString result = "\"";
        QString s = x.toString();
        for(int i=0; i<s.size(); i++)
        {
            result += escapeChar(s[i]);
        }
        result += "\"";
        return result;
    }
    case QVariant::Invalid:
    {
        return "invalid";
    }
    case QVariant::List:
    {
        QList<QVariant> list = x.toList();
        QString result = "[";
        for(int i=0; i<list.size(); i++)
        {
            if(i>0) result += ", ";
            result += variantToText(list[i]);
        }
        result += "]";
        return result;
    }
    case QVariant::Char:
    {
        QChar c = x.toChar();
        return "'" + escapeChar(c) + "'";
    }
    case QVariant::StringList:
    {
        QStringList list = x.toStringList();
        QString result = "[";
        for(int i=0; i<list.size(); i++)
        {
            if(i>0) result += ", ";
            result += variantToText(list[i]);
        }
        result += "]";
        return result;
    }
    //case QVariant::Bool:
    //case QVariant::Int:
    //case QVariant::UInt:
    //case QVariant::LongLong:
    //case QVariant::ULongLong:
    //case QVariant::Double:
    case QVariant::ByteArray:
    case QVariant::BitArray:
    //case QVariant::Date:
    //case QVariant::Time:
    //case QVariant::DateTime:
    case QVariant::Url:
    case QVariant::Locale:
    case QVariant::Rect:
    case QVariant::RectF:
    case QVariant::Size:
    case QVariant::SizeF:
    case QVariant::Line:
    case QVariant::LineF:
    case QVariant::Point:
    case QVariant::PointF:
    case QVariant::RegExp:
    case QVariant::RegularExpression:
    case QVariant::Hash:
#if QT_CONFIG(easingcurve)
    case QVariant::EasingCurve:
#endif
    case QVariant::Uuid:
#if QT_CONFIG(itemmodel)
    case QVariant::ModelIndex:
    case QVariant::PersistentModelIndex:
#endif
    case QVariant::LastCoreType:
    case QVariant::Font:
    case QVariant::Pixmap:
    case QVariant::Brush:
    case QVariant::Color:
    case QVariant::Palette:
    case QVariant::Image:
    case QVariant::Polygon:
    case QVariant::Region:
    case QVariant::Bitmap:
    case QVariant::Cursor:
    case QVariant::KeySequence:
    case QVariant::Pen:
    case QVariant::TextLength:
    case QVariant::TextFormat:
    case QVariant::Matrix:
    case QVariant::Transform:
    case QVariant::Matrix4x4:
    case QVariant::Vector2D:
    case QVariant::Vector3D:
    case QVariant::Vector4D:
    case QVariant::Quaternion:
    case QVariant::PolygonF:
    case QVariant::Icon:
    case QVariant::LastGuiType:
    case QVariant::SizePolicy:
    case QVariant::UserType:
    default:
        return x.toString();
        break;
    }
}
#endif // VARIANTTOTEXT_H
