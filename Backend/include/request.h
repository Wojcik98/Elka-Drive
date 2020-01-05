#ifndef REQUEST_H
#define REQUEST_H

#include <QNetworkRequest>
#include "include/response.h"

class Request {
public:
    enum Method {
        GET, POST, DELETE
    };

    Request(Method method, QNetworkRequest request, Response::Type type);
    void setData(QByteArray data);
    Method getMethod();
    QNetworkRequest getRequest();
    Response::Type getType();
    QByteArray getData();

private:
    Method method;
    QNetworkRequest request;
    Response::Type type;
    QByteArray data;
};

#endif // REQUEST_H
