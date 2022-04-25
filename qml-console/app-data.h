#ifndef APPDATA_H
#define APPDATA_H
#include <QtCore>
#include <QtQml>
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
static inline QString jsValueToText(const QJSValue &x)
{
    if(x.isVariant())
    {
        return x.toVariant().toString();
    }
    else if(x.isQObject())
    {
        QObject *qobj = x.toQObject();
        return QString("QObject(%1)").arg(qobj->metaObject()->className());
    }
    else if(x.isRegExp())
    {
        return x.toString();
    }
    else if(x.isDate())
    {
        return QString("Date(%1)").arg(x.toDateTime().toString("yyyy-MM-ddThh:mm:ss.z"));
    }
    else if(x.isArray())
    {
        const QJSValue &array = x;
        QString result = "[";
        qint32 length = array.property("length").toInt();
        qint32 i;
        for(i = 0; i < length; i++)
        {
            if(i>0) result += ", ";
            result += jsValueToText(array.property(i));
        }
        QJSValueIterator it(array);
        while(it.hasNext()) {
            it.next();
            QString name = it.name();
            if(name == "length") continue;
            bool ok;
            name.toInt(&ok);
            if(ok) continue;
            if(i>0) result += ", ";
            result += jsValueToText(name);
            result += ": ";
            result += jsValueToText(it.value());
            i++;
        }
        result += "]";
        return result;
    }
    else if(x.isObject())
    {
        if(x.toString().startsWith("function"))
        {
            QString name = x.property("name").toString();
            if(name.isEmpty()) return "<anonymous function>";
            return QString("<function %1>").arg(name);
        }
        const QJSValue &object = x;
        QJSValueIterator it(object);
        QString result = "{";
        qint64 i = 0;
        while(it.hasNext()) {
            it.next();
            if(i>0) result += ", ";
            QString name = it.name();
            result += escapeString(name);
            result += ": ";
            result += jsValueToText(it.value());
            i++;
        }
        result += "}";
        return result;
    }
    else if(x.isString())
    {
        return escapeString(x.toString());
    }
    else
    {
        return x.toString();
    }
}
class ApplicationData : public QObject
{
    Q_OBJECT
    int m_count = 1234;
public:
    Q_PROPERTY(int count READ count WRITE setCount);
    explicit ApplicationData(QObject *parent = nullptr) : QObject(parent) {
        qDebug() << "ApplicationData::ApplicationData() called";
    }
    virtual ~ApplicationData()
    {
        qDebug() << "~ApplicationData() called";
    }
    Q_INVOKABLE QString getTextFromCpp(){
        return QString("This is the text from C++");
    }
    Q_INVOKABLE QVariant retVariant()
    {
        QStringList sl = {"abc", "xyz"};
        return sl;
    }
    Q_INVOKABLE QVariant retVariant2()
    {
        QVariantMap map;
        QVariantList sl = {"abc", false, "xyz", QVariant()};
        sl.push_back(777);
        QVariant x, y(QString()), z(QString(""));
        x.convert(QMetaType::Int);
        //sl.push_back(QVariant((const QObject *)nullptr));
        sl.push_back(x);
        map["a"] = 11;
        map["b"] = sl;
        return map;
    }
    int count() const
    {
        return m_count;
    }
    void setCount(int value)
    {
        if (m_count != value) {
            m_count = value;
        }
    }
    Q_INVOKABLE QString dump() const
    {
        return QString("ApplicationData(count=%1)").arg(m_count);
    }
};
class ApplicationFactory : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationFactory(QObject *parent = nullptr) : QObject(parent) {
    }
    Q_INVOKABLE ApplicationData *newApplicationData(){
        return new class ApplicationData ();
    }
    Q_INVOKABLE void log(const QJSValue &x)
    {
        qDebug().noquote() << jsValueToText(x);
    }
};
#endif // APPDATA_H
