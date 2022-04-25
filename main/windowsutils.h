#ifndef WINDOWSUTILS_H
#define WINDOWSUTILS_H
#include <QtCore>
class WindowsUtils
{
public:
    WindowsUtils();
    static bool ShowWindow(const qint64 &pidQt);
    static bool MinimizeWindow(const qint64 &pidQt);
    static bool RestoreWindow(const qint64 &pidQt);
};
#endif // WINDOWSUTILS_H
