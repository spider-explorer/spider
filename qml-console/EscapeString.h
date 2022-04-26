#ifndef ESCAPESTRING_H
#define ESCAPESTRING_H
#include <QtCore>
static inline QString escapeChar(const QChar &c)
{
    // https://en.cppreference.com/w/cpp/language/escape
    wchar_t wc = c.unicode();
    switch(wc)
    {
    case L'"':
        return "\\\"";
    case L'\\':
        return "\\\\";
    case L'\a':
        return "\\a";
    case L'\b':
        return "\\b";
    case L'\f':
        return "\\f";
    case L'\n':
        return "\\n";
    case L'\r':
        return "\\r";
    case L'\t':
        return "\\t";
    case L'\v':
        return "\\v";
    default:
        return QString(c);
        break;
    }
}
static inline QString escapeString(const QString &s)
{
    QString result = "\"";
    for(int i=0; i<s.size(); i++)
    {
        result += escapeChar(s[i]);
    }
    result += "\"";
    return result;
}
#endif // ESCAPESTRING_H
