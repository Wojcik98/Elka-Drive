#ifndef REQUEST_H
#define REQUEST_H

#include <QNetworkRequest>
#include "include/requesttype.h"

class Request {
public:
    enum Method {
        GET, POST, DELETE
    };

    Request(const Method &method, const QNetworkRequest &request, const RequestType &type);
    void setData(const QByteArray &data);
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
