#include "jarchiver.h"
#include "common.h"
#include "jsonserializer.h"
#include "mainwindow.h"
#include "spidercore.h"
#include "spidermain.h"
#include "variantserializer.h"
#include <QApplication>
#include <QtNetwork>
class SpiderApplication : public QApplication
{
    Q_OBJECT
public:
    SpiderApplication(int &argc, char **argv) : QApplication(argc, argv)
    {
    }
    void handele_entry2()
    {
        emit signal_entry2();
    }
signals:
    void signal_entry2();
};
extern "C" __declspec(dllexport) void entry1(const char *main_dll_path, int argc, char **argv)
{
    SpiderApplication app(argc, argv);
    qDebug() << "entry1() called." << main_dll_path;
    auto font = app.font();
    qDebug() << font.pointSizeF();
    font.setPointSizeF(12.0);
    app.setFont(font);
    QPixmap pixmap(":/a-hand-with-a-paintbrush-sticks-out-of-the-laptop-screen.png");
    QSplashScreen splash(pixmap);
    splash.show();
    SpiderCore core(splash, main_dll_path);
    SpiderMain w;
    QObject::connect(&app, &SpiderApplication::signal_entry2, &w, &SpiderMain::on__signal_entry2);
    w.show();
    splash.finish(&w);
    exit(app.exec());
}
extern "C" __declspec(dllexport) void entry2()
{
    qDebug() << "entry2(begin)";
    ((SpiderApplication *)qApp)->handele_entry2();
#if 0x0
    w.showMaximized();
    w.activateWindow();
    w.raise();
    // w.setWindowState(Qt::WindowNoState | Qt::WindowActive);
    w.setWindowState(Qt::WindowMaximized | Qt::WindowActive);
#endif
}
#include "entry.moc"
