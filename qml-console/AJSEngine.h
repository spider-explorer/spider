#ifndef AJSENGINE_H
#define AJSENGINE_H
#include <QtCore>
//#include <QJSEngine>
#include <QJSValueIterator>
#include "EscapeString.h"
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
#if 0x0
class AJSEngineConsole : public QObject
{
    Q_OBJECT
public:
    explicit AJSEngineConsole(QObject *parent = nullptr) : QObject(parent)
    {
    }
    Q_INVOKABLE void log(const QJSValue &x)
    {
        //qDebug() << "log() called!";
        qDebug().noquote() << jsValueToText(x);
    }
};
class AJSEngine : public QJSEngine
{
    Q_OBJECT
    QJSValue m_debug;
    QJSValue m_stringify;
public:
    AJSEngineConsole m_console;
    explicit AJSEngine(QObject *parent = nullptr) : QJSEngine(parent)
    {
        this->installExtensions(QJSEngine::AllExtensions); // https://runebook.dev/ja/docs/qt/qjsengine
        m_debug = this->evaluate("(function debug(x) { console.log(JSON.stringify(x)); })");
        this->globalObject().setProperty("debug", m_debug);
        this->globalObject().property("console").setProperty("dir", m_debug);
        m_stringify = this->evaluate("(function stringify(x) { return JSON.stringify(x); })");
        this->globalObject().setProperty("stringify", m_stringify);
        this->globalObject().setProperty("myconsole", this->newQObject(new AJSEngineConsole));
    }
    virtual ~AJSEngine()
    {
    }
    QJSValue evaluateFile(const QString &fileName)
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly);
        return this->evaluate(file.readAll(), fileName);
    }
    void debug(const QJSValue &x)
    {
        m_debug.call({x});
    }
    QString stringify(const QJSValue &x)
    {
        return m_stringify.call({x}).toString();
    }
};
#endif
#endif // AJSENGINE_H
