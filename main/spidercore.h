#ifndef SPIDERCORE_H
#define SPIDERCORE_H
#include "favmanager.h"
#include "jsonsettings.h"
#include "recursivefilesystemwatcher.h"
#include <QtCore>
#include <QtGui>
#include <QtNetwork>
#include <QtWidgets>
enum ProcStage
{
    SETUP,
    FINISH
};
using ProcCallback = std::function<void (QProcess &proc, ProcStage stage, bool success)>;
class SpiderCore
{
public:
    explicit SpiderCore(QSplashScreen &splash, const QString &mainDllPath);
    QSplashScreen &splash();
    QMap<QString, QString> &env();
    JsonSettings &settings();
    QString selectedRepoName();
    QString selectedMsys2Name();
    void open_nyagos(QWidget *widget, QString path = "");
    void open_bash(QWidget *widget, QString path = "");
    void open_file(QWidget *widget, QString path);
    void open_notepad3(QWidget *widget, QString path);
    QMessageBox::StandardButton check_system_qt_project(QWidget *widget, QString proFile);
    void develop_with_qtcreator(QWidget *widget, QString proFile);
    void develop_with_lazarus(QWidget *widget, QString lprFile);
    void develop_with_codelite(QWidget *widget, QString projFile);
    void develop_with_geany(QWidget *widget, QString path);
    void open_qt_dir(QWidget *widget, QString path);
    void open_explorer(QWidget *widget, QString repo);
    RecursiveFileSystemWatcher &watcher();
    void remove_repo(QWidget *widget, QString repoDir);
    void refresh_repo(QWidget *widget, QString repoDir);
    void open_msys2(QWidget *widget, QString msys2Dir, QString currentDir = "");
    void install_msys2(QWidget *widget);
    void remove_msys2(QWidget *widget, QString name);
    void open_git_page(QWidget *widget, QString repoDir);
    void open_vscode(QWidget *widget, QString repoDir);
    void open_smartgit(QWidget *widget, QString repoDir);
private:
    QMap<QString, QString> m_env;
    QSplashScreen &m_splash;
    RecursiveFileSystemWatcher m_watcher;
    JsonSettings m_settings;
    QMutex m_mutex;
    QSplashScreen m_one_moment;
private:
////void prepareScoop();
    QString prepareProgram(JsonSettings &softwareSettings, QString progName);
    QString prepareWsl(QString distroName);
};
extern SpiderCore &g_core();
extern JsonSettings &g_settings();
extern QMutex &g_mutex();
#endif // SPIDERCORE_H
