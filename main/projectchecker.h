#ifndef PROJECTCHECKER_H
#define PROJECTCHECKER_H
#include <QtCore>
class ProjectChecker
{
public:
    explicit ProjectChecker(QString dir);
    bool isHome();
    QString getQtProjectFile();
    bool isGitDir();
    bool isQtProject(bool recursive = false);
    bool isQtBuildDir();
    bool isDartProject();
    bool isJavaProject();
    bool isVisible();
private:
    QString m_dir;
};
#endif // PROJECTCHECKER_H
