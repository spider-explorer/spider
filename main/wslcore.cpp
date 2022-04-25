#include "wslcore.h"
#include "3rdparty/EasyOLE.h"
#include "3rdparty/WinReg.hpp"
#include "3rdparty/prettyprint.hpp"
#include "wslapi.h"
#include <QtCore>
#include <iostream>
WslCore::WslCore()
{
}
void WslCore::test()
{
    std::wstring wsl_root = LR"(Software\Microsoft\Windows\CurrentVersion\Lxss)";
    winreg::RegKey wsl{HKEY_CURRENT_USER, wsl_root};
    std::vector<std::wstring> keys = wsl.EnumSubKeys();
    std::wcout << keys << std::endl;
    QStringList dnameList;
    // this->ui->listWidget->clear();
    for (const auto &key : keys)
    {
        std::wcout << key << std::endl;
        winreg::RegKey wsl2{HKEY_CURRENT_USER, wsl_root + L"\\" + key};
        std::optional<std::wstring> dname = wsl2.TryGetStringValue(L"DistributionName");
        std::wcout << dname.value_or(L"<null>") << std::endl;
        if (dname.has_value())
        {
            dnameList.append(QString::fromStdWString(dname.value()));
        }
    }
    qDebug() << dnameList;
}
QStringList WslCore::distroList()
{
    std::wstring wsl_root = LR"(Software\Microsoft\Windows\CurrentVersion\Lxss)";
    winreg::RegKey wsl{HKEY_CURRENT_USER, wsl_root};
    std::vector<std::wstring> keys = wsl.EnumSubKeys();
    // std::wcout << keys << std::endl;
    QStringList dnameList;
    // this->ui->listWidget->clear();
    for (const auto &key : keys)
    {
        // std::wcout << key << std::endl;
        winreg::RegKey wsl2{HKEY_CURRENT_USER, wsl_root + L"\\" + key};
        std::optional<std::wstring> dname = wsl2.TryGetStringValue(L"DistributionName");
        // std::wcout << dname.value_or(L"<null>") << std::endl;
        if (dname.has_value())
        {
            dnameList.append(QString::fromStdWString(dname.value()));
        }
    }
    return dnameList;
}
QMap<QString, QString> WslCore::guiAppMap(QString distro)
{
    qDebug() << "WslCore::guiAppMap():" << distro;
    QString menuRoot = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) +
                       "/AppData/Roaming/Microsoft/Windows/Start Menu/Programs";
    QString path = menuRoot + "/" + distro;
    qDebug() << path;
    QMap<QString, QString> result;
    QDirIterator links(path);
    while (links.hasNext())
    {
        QString link = links.next();
        QFileInfo linkInfo(link);
        if (linkInfo.fileName() == "." || linkInfo.fileName() == "..")
            continue;
        // qDebug() << link;
        // qDebug() << linkInfo.completeBaseName();
        QString linkName = linkInfo.completeBaseName();
        result[linkName] = linkInfo.absoluteFilePath();
    }
    return result;
}
QMap<QString, QString> WslCore::linkMap(QString linkPath)
{
    QMap<QString, QString> result;
    // std::wstring result;
    IDispatch *wshell;
    IDispatch *shortcut;
    HRESULT hResult;
    VARIANT v1, v2;
    VARIANT sc;
    std::wstring link = linkPath.toStdWString();
    hResult = EasyOleCreateInstance(L"WScript.Shell", &wshell);
    if (!SUCCEEDED(hResult))
        return result;
    V_VT(&v1) = VT_BSTR;
    V_BSTR(&v1) = ::SysAllocString(link.c_str());
    hResult = EasyOleMethodCall(wshell, &sc, (BSTR)L"CreateShortcut", 1, v1, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    VariantClear(&v1);
    shortcut = V_DISPATCH(&sc);
    //
    hResult = EasyOlePropertyGet(shortcut, &v2, (BSTR)L"TargetPath", 0, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                 NULL_VARIANT, NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    result["TargetPath"] = QString::fromWCharArray(v2.bstrVal);
    VariantClear(&v2);
    //
    hResult = EasyOlePropertyGet(shortcut, &v2, (BSTR)L"Arguments", 0, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                 NULL_VARIANT, NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    QString Arguments = QString::fromWCharArray(v2.bstrVal);
    QStringList ArgumentList = this->parseArguments(Arguments);
    result["Arguments"] = ArgumentList.join("|");
    VariantClear(&v2);
    //
    hResult = EasyOlePropertyGet(shortcut, &v2, (BSTR)L"Description", 0, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                 NULL_VARIANT, NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    result["Description"] = QString::fromWCharArray(v2.bstrVal);
    VariantClear(&v2);
    //
    hResult = EasyOlePropertyGet(shortcut, &v2, (BSTR)L"WorkingDirectory", 0, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                 NULL_VARIANT, NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    result["WorkingDirectory"] = QString::fromWCharArray(v2.bstrVal);
    VariantClear(&v2);
    //
    hResult = EasyOlePropertyGet(shortcut, &v2, (BSTR)L"IconLocation", 0, NULL_VARIANT, NULL_VARIANT, NULL_VARIANT,
                                 NULL_VARIANT, NULL_VARIANT);
    if (!SUCCEEDED(hResult))
        return result;
    QString IconLocation = QString::fromWCharArray(v2.bstrVal);
    if (IconLocation.split(",").size() > 0)
    {
        IconLocation = IconLocation.split(",")[0];
    }
    result["IconLocation"] = IconLocation;
    VariantClear(&v2);
    //
    if (shortcut)
        shortcut->Release();
    if (wshell)
        wshell->Release();
    return result;
}
QStringList WslCore::parseArguments(QString arguments)
{
    // qDebug() << "WslCore::parseArguments():" << arguments;
    QStringList result;
    // result = arguments.split(" ");
    QRegExp rx(R"***(("[^"]+"|[^\s"]+))***");
    int pos = 0;
    while ((pos = rx.indexIn(arguments, pos)) != -1)
    {
        result << rx.cap(0).replace("\"", "");
        pos += rx.matchedLength();
    }
    // qDebug() << result;
    return result;
}
