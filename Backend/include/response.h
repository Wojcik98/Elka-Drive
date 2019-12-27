#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>

class Response {
public:
    enum Type {
        LOGIN,
        REGISTER,
        GROUPS
    };

    Response(int statusCode, QByteArray body, Response::Type type);
    int getStatus();
    QByteArray getBody();
    Response::Type getType();

private:
    int statusCode;
    QByteArray body;
    Response::Type type;
};

#endif // RESPONSE_H
