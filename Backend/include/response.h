#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>
#include <QString>
#include "include/requesttype.h"

class Model;

class Response {
public:
    Response(QByteArray body, RequestType type);
    QByteArray getBody();
    RequestType getType();

private:
    QByteArray body;
    RequestType type;
};

#endif // RESPONSE_H
