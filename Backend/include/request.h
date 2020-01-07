#ifndef REQUEST_H
#define REQUEST_H

#include <QNetworkRequest>
#include "include/requesttype.h"

class Request {
public:
    enum Method {
        GET, POST, DELETE
    };

    Request(Method method, QNetworkRequest request, RequestType type);
    void setData(QByteArray data);
    Method getMethod();
    QNetworkRequest getRequest();
    RequestType getType();
    QByteArray getData();

private:
    Method method;
    QNetworkRequest request;
    RequestType type;
    QByteArray data;
};

#endif // REQUEST_H
