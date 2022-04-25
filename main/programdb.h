#ifndef PROGRAMDB_H
#define PROGRAMDB_H
#include <QtCore>
class ProgramDB
{
public:
    ProgramDB();
    QString which(const QString progName);
    QFileInfo android_studio() const;
    QFileInfo idea() const;
    QFileInfo emacs() const;
private:
    QFileInfo m_android_studio;
    QFileInfo m_idea;
    QFileInfo m_emacs;
};
#endif // PROGRAMDB_H
