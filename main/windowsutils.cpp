#include "windowsutils.h"
#include <windows.h>
static int g_winState = SW_SHOW;
static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    // get the window process ID
    DWORD searchedProcessId = (DWORD)lParam;
    DWORD windowProcessId = 0;
    GetWindowThreadProcessId(hWnd, &windowProcessId);
    // check the process id match
    if (windowProcessId == searchedProcessId)
    {
        ShowWindow(hWnd, g_winState);
        return FALSE;
    }
    return TRUE; // continue enumeration
}
WindowsUtils::WindowsUtils()
{
}
bool WindowsUtils::ShowWindow(const qint64 &pidQt)
{
    g_winState = SW_SHOW;
    return EnumWindows(EnumWindowsProc, (LPARAM)pidQt);
}
bool WindowsUtils::MinimizeWindow(const qint64 &pidQt)
{
    g_winState = SW_MINIMIZE;
    return EnumWindows(EnumWindowsProc, (LPARAM)pidQt);
}
bool WindowsUtils::RestoreWindow(const qint64 &pidQt)
{
    g_winState = SW_RESTORE;
    return EnumWindows(EnumWindowsProc, (LPARAM)pidQt);
}
