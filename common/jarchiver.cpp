#include "jarchiver.h"
// https://github.com/libarchive/libarchive/wiki/Examples#A_Universal_Decompressor
static int copy_data(struct archive *ar, struct archive *aw)
{
    int r;
    const void *buff;
    size_t size;
    la_int64_t offset;
    for (;;)
    {
        r = archive_read_data_block(ar, &buff, &size, &offset);
        if (r == ARCHIVE_EOF)
            return (ARCHIVE_OK);
        if (r < ARCHIVE_OK)
            return (r);
        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK)
        {
            fprintf(stderr, "%s\n", archive_error_string(aw));
            return (r);
        }
    }
}
bool extract_archive(const QString &archive_path, const QString &output_path, ArchiveIdleCallback callback)
{
    qint64 extractSizeTotal = 0;
    QDir dir(output_path);
    dir.removeRecursively();
    int r;
    struct archive *a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    struct archive *ext = archive_write_disk_new();
    int flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    r = archive_read_open_filename_w(a, archive_path.toStdWString().c_str(), 10240);
    if (r)
    {
        qDebug() << "Could not open:" << archive_path;
        return false;
    }
    for (;;)
    {
        struct archive_entry *entry;
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(a));
        if (r < ARCHIVE_WARN)
            return false;
        QString entry_pathname = QString::fromWCharArray(archive_entry_pathname_w(entry));
        // qDebug() << "entry_pathname:" << entry_pathname;
        la_int64_t entry_size = archive_entry_size(entry);
        // qDebug() << "entry_size:" << entry_size;
        time_t mtime = archive_entry_mtime(entry);
#if 0x0
        QDateTime mtime_dt = QDateTime::fromTime_t(mtime);
#else
        QDateTime mtime_dt;
        mtime_dt.setSecsSinceEpoch(mtime);
#endif
        // qDebug() << "mtime:" << mtime_dt;
        QString expFilePath = output_path + QString("/") + entry_pathname;
        // qDebug() << "expFilePath:" << expFilePath;
        archive_entry_set_pathname(entry, expFilePath.toLocal8Bit().constData());
        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
        {
            fprintf(stderr, "%s\n", archive_error_string(ext));
        }
        //else if (archive_entry_size(entry) > 0)
        else if (entry_size > 0)
        {
            r = copy_data(a, ext);
            if (r < ARCHIVE_OK)
                fprintf(stderr, "%s\n", archive_error_string(ext));
            if (r < ARCHIVE_WARN)
                return false;
            extractSizeTotal += entry_size;
            if(callback != nullptr) callback(extractSizeTotal);
        }
        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            fprintf(stderr, "%s\n", archive_error_string(ext));
        if (r < ARCHIVE_WARN)
            return false;
    }
    archive_read_close(a);
    archive_read_free(a);
    return true;
}
