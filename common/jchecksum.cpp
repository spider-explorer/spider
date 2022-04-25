#include "jchecksum.h"
QByteArray bytesChecksum(const QByteArray &bytes, QCryptographicHash::Algorithm hashAlgoritm)
{
    QCryptographicHash hash(hashAlgoritm);
    hash.addData(bytes);
    return hash.result().toHex();
}
QByteArray fileChecksum(const QString &path, QCryptographicHash::Algorithm hashAlgoritm)
{
    QFile f(path);
    if (f.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(hashAlgoritm);
        if (hash.addData(&f))
        {
            return hash.result().toHex();
        }
    }
    return QByteArray();
}
