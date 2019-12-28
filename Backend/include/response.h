#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>
#include <QString>

class Response {
public:
    enum Type {
        LOGIN,
        REGISTER,
        GROUPS,
        PATH,
        FILE
    };

    Response(int statusCode, QByteArray body, Response::Type type, QString name);
    int getStatus();
    QByteArray getBody();
    Response::Type getType();
    QString getName();

private:
    int statusCode;
    QByteArray body;
    Response::Type type;
    QString name;
};

#endif // RESPONSE_H
