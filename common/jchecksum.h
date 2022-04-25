#ifndef JCHECKSUM_H
#define JCHECKSUM_H
#include <QtCore>
QByteArray bytesChecksum(const QByteArray &bytes, QCryptographicHash::Algorithm hashAlgoritm);
QByteArray fileChecksum(const QString &path, QCryptographicHash::Algorithm hashAlgoritm);
#endif // JCHECKSUM_H
