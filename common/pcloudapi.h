#ifndef PCLOUDAPI_H
#define PCLOUDAPI_H
#include <QtCore>
#include "jnetwork.h"
class pCloudAPI
{
public:
    pCloudAPI();
    ~pCloudAPI();
    void listFolder();
private:
    JNetworkManager m_nm;
    QString m_auth;
};
#endif // PCLOUDAPI_H
