#ifndef JARCHIVER_H
#define JARCHIVER_H
#include <QtCore>
#include <archive.h>
#include <archive_entry.h>
#include <sys/utime.h>
using ArchiveIdleCallback = std::function<void (qint64 extractSizeTotal)>;
bool extract_archive(const QString &archive_path,
                     const QString &output_path,
                     ArchiveIdleCallback callback = nullptr);
#endif // JARCHIVER_H
