#ifndef UTF8LOGHANDLER_H
#define UTF8LOGHANDLER_H
#include <QtCore>
#include <windows.h>
static void utf8LogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    const auto &message = qFormatLogMessage(type, context, msg);
    QTextStream cout(stderr);
    if (GetConsoleOutputCP() == CP_UTF8)
    {
#if QT_VERSION >= 0x060000
        cout.setEncoding(QStringConverter::Utf8);
#else
        cout.setCodec("UTF-8");
#endif
    }
    cout << message << Qt::endl << Qt::flush;
}
#endif // UTF8LOGHANDLER_H
