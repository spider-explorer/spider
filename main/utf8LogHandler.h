#ifndef UTF8LOGHANDLER_H
#define UTF8LOGHANDLER_H
#include <QtCore>
#include <windows.h>
static void utf8LogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const auto &message = qFormatLogMessage(type, context, msg);
    QTextStream cerr(stderr);
    if (GetConsoleOutputCP() == CP_UTF8)
    {
#if QT_VERSION >= 0x060000
        cerr.setEncoding(QStringConverter::Utf8);
#else
        cerr.setCodec("UTF-8");
#endif
    }
#if QT_VERSION >= 0x060000
    else
    {
        cerr.setEncoding(QStringConverter::System);
    }
#endif
    cerr << message << Qt::endl << Qt::flush;
}
#endif // UTF8LOGHANDLER_H
