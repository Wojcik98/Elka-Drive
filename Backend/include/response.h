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
        NEW_GROUP,
        PATH,
        FILE,
        GROUP_USERS,
        GROUP_DELETE,
        GROUP_ADD_USER,
        GROUP_REMOVE_USER
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
