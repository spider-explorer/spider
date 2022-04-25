#include "junctionmanager.h"

#include <windows.h>

JunctionManager::JunctionManager()
{
}

/* The following structure was copied from
   http://msdn.microsoft.com/en-us/library/ff552012.aspx as the required
   include km\ntifs.h isn't present in the Windows SDK (at least as included
   with Visual Studio Express). Use unique names to avoid conflicting with
   the structure as defined by Min GW. */
typedef struct {
    ULONG ReparseTag;
    USHORT ReparseDataLength;
    USHORT Reserved;
    union {
        struct {
            USHORT SubstituteNameOffset;
            USHORT SubstituteNameLength;
            USHORT PrintNameOffset;
            USHORT PrintNameLength;
            ULONG Flags;
            WCHAR PathBuffer[1];
        } SymbolicLinkReparseBuffer;

        struct {
            USHORT SubstituteNameOffset;
            USHORT  SubstituteNameLength;
            USHORT  PrintNameOffset;
            USHORT  PrintNameLength;
            WCHAR  PathBuffer[1];
        } MountPointReparseBuffer;

        struct {
            UCHAR  DataBuffer[1];
        } GenericReparseBuffer;
    };
} _Py_REPARSE_DATA_BUFFER, *_Py_PREPARSE_DATA_BUFFER;

#define _Py_REPARSE_DATA_BUFFER_HEADER_SIZE \
    FIELD_OFFSET(_Py_REPARSE_DATA_BUFFER, GenericReparseBuffer)
#define _Py_MAXIMUM_REPARSE_DATA_BUFFER_SIZE  ( 16 * 1024 )

static bool
CreateJunction(LPCWSTR src_path, LPCWSTR dst_path)
{
    /* Privilege adjustment */
    HANDLE token = NULL;
    TOKEN_PRIVILEGES tp;

    /* Reparse data buffer */
    const USHORT prefix_len = 4;
    USHORT print_len = 0;
    USHORT rdb_size = 0;
    _Py_PREPARSE_DATA_BUFFER rdb = NULL;

    /* Junction point creation */
    HANDLE junction = NULL;
    DWORD ret = 0;

    if (src_path == NULL || dst_path == NULL)
        return false;

    if (wcsncmp(src_path, L"\\??\\", prefix_len) == 0)
        return false;

    /* Adjust privileges to allow rewriting directory entry as a
       junction point. */
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
        goto cleanup;

    if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &tp.Privileges[0].Luid))
        goto cleanup;

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(TOKEN_PRIVILEGES),
                               NULL, NULL))
        goto cleanup;

    if (GetFileAttributesW(src_path) == INVALID_FILE_ATTRIBUTES)
        goto cleanup;

    /* Store the absolute link target path length in print_len. */
    print_len = (USHORT)GetFullPathNameW(src_path, 0, NULL, NULL);
    if (print_len == 0)
        goto cleanup;

    /* NUL terminator should not be part of print_len. */
    --print_len;

    /* REPARSE_DATA_BUFFER usage is heavily under-documented, especially for
       junction points. Here's what I've learned along the way:
       - A junction point has two components: a print name and a substitute
         name. They both describe the link target, but the substitute name is
         the physical target and the print name is shown in directory listings.
       - The print name must be a native name, prefixed with "\??\".
       - Both names are stored after each other in the same buffer (the
         PathBuffer) and both must be NUL-terminated.
       - There are four members defining their respective offset and length
         inside PathBuffer: SubstituteNameOffset, SubstituteNameLength,
         PrintNameOffset and PrintNameLength.
       - The total size we need to allocate for the REPARSE_DATA_BUFFER, thus,
         is the sum of:
         - the fixed header size (REPARSE_DATA_BUFFER_HEADER_SIZE)
         - the size of the MountPointReparseBuffer member without the PathBuffer
         - the size of the prefix ("\??\") in bytes
         - the size of the print name in bytes
         - the size of the substitute name in bytes
         - the size of two NUL terminators in bytes */
    rdb_size = _Py_REPARSE_DATA_BUFFER_HEADER_SIZE +
        sizeof(rdb->MountPointReparseBuffer) -
        sizeof(rdb->MountPointReparseBuffer.PathBuffer) +
        /* Two +1's for NUL terminators. */
        (prefix_len + print_len + 1 + print_len + 1) * sizeof(WCHAR);
    rdb = (_Py_PREPARSE_DATA_BUFFER)calloc(1, rdb_size);
    if (rdb == NULL)
        goto cleanup;

    rdb->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
    rdb->ReparseDataLength = rdb_size - _Py_REPARSE_DATA_BUFFER_HEADER_SIZE;
    rdb->MountPointReparseBuffer.SubstituteNameOffset = 0;
    rdb->MountPointReparseBuffer.SubstituteNameLength =
        (prefix_len + print_len) * sizeof(WCHAR);
    rdb->MountPointReparseBuffer.PrintNameOffset =
        rdb->MountPointReparseBuffer.SubstituteNameLength + sizeof(WCHAR);
    rdb->MountPointReparseBuffer.PrintNameLength = print_len * sizeof(WCHAR);

    /* Store the full native path of link target at the substitute name
       offset (0). */
    wcscpy(rdb->MountPointReparseBuffer.PathBuffer, L"\\??\\");
    if (GetFullPathNameW(src_path, print_len + 1,
                         rdb->MountPointReparseBuffer.PathBuffer + prefix_len,
                         NULL) == 0)
        goto cleanup;

    /* Copy everything but the native prefix to the print name offset. */
    wcscpy(rdb->MountPointReparseBuffer.PathBuffer +
             prefix_len + print_len + 1,
             rdb->MountPointReparseBuffer.PathBuffer + prefix_len);

    /* Create a directory for the junction point. */
    if (!CreateDirectoryW(dst_path, NULL))
        goto cleanup;

    junction = CreateFileW(dst_path, GENERIC_READ | GENERIC_WRITE, 0, NULL,
        OPEN_EXISTING,
        FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (junction == INVALID_HANDLE_VALUE)
        goto cleanup;

    /* Make the directory entry a junction point. */
    if (!DeviceIoControl(junction, FSCTL_SET_REPARSE_POINT, rdb, rdb_size,
                         NULL, 0, &ret, NULL))
        goto cleanup;

cleanup:
    ret = GetLastError();

    CloseHandle(token);
    CloseHandle(junction);
    free(rdb);

    if (ret != 0)
        return false;

    return true;
}


bool JunctionManager::create(const QString &dest, const QString &src)
{
    return ::CreateJunction(src.toStdWString().c_str(), dest.toStdWString().c_str());
}

bool JunctionManager::remove(const QString &dest)
{
    return QDir(dest).rmdir(".");
}
