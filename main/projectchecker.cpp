#include "projectchecker.h"
#include "chooseqtprojectdialog.h"
ProjectChecker::ProjectChecker(QString dir) : m_dir(dir)
{
}
bool ProjectChecker::isHome()
{
    if (QFile(m_dir + "/.nyagos").exists())
        return true;
    if (QFile(m_dir + "/_nyagos").exists())
        return true;
    if (QFile(m_dir + "/.profile").exists())
        return true;
    return false;
}
QString ProjectChecker::getQtProjectFile()
{
    QDir::Filters filters = QDir::Files;
    QDirIterator::IteratorFlags flags = QDirIterator::Subdirectories;
    QDirIterator it(m_dir, filters, flags);
    QStringList proList;
    while (it.hasNext())
    {
        QString proFile = it.next();
        if (proFile.endsWith(".pro"))
        {
            QDir dir(m_dir);
            // proFile = dir.relativeFilePath(proFile);
            proList.append(proFile);
        }
    }
    qDebug() << proList;
#if 0x0
    if (proList.size() == 1)
    {
        qDebug() << proList;
        return proList[0];
    }
#endif
    ChooseQtProjectDialog dlg(m_dir, proList);
    if (!dlg.exec())
    {
        return "";
    }
    return dlg.proFile();
}
bool ProjectChecker::isGitDir()
{
    if (QDir(m_dir + "/.git").exists())
        return true;
    return false;
}
bool ProjectChecker::isQtProject(bool recursive)
{
    QDir::Filters filters = QDir::Files;
    QDirIterator::IteratorFlags flags = recursive ? QDirIterator::Subdirectories : QDirIterator::NoIteratorFlags;
    QDirIterator it(m_dir, filters, flags);
    QStringList proList;
    while (it.hasNext())
    {
        QString proFile = it.next();
        if (proFile.endsWith(".pro"))
        {
            proList.append(proFile);
        }
    }
    return (proList.size() > 0);
}
bool ProjectChecker::isQtBuildDir()
{
    if (QFileInfo(m_dir + "/.qmake.stash").exists())
        return true;
    return false;
}
bool ProjectChecker::isDartProject()
{
    QDir::Filters filters = QDir::Files;
    QDirIterator::IteratorFlags flags = QDirIterator::NoIteratorFlags;
    QString repoDirLib = m_dir + "/lib";
    QDirIterator it1(repoDirLib, filters, flags);
    while (it1.hasNext())
    {
        QString dartFile = it1.next();
        if (dartFile.endsWith(".dart"))
        {
            return true;
        }
    }
    return false;
}
bool ProjectChecker::isJavaProject()
{
    if (QFile(m_dir + "/build.gradle").exists())
        return true;
    if (QFile(m_dir + "/pom.xml").exists())
        return true;
    return false;
}
bool ProjectChecker::isVisible()
{
    return !this->isQtBuildDir();
}
