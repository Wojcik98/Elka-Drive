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
        GROUP_USERS,
        GROUP_DELETE,
        GROUP_ADD_USER,
        GROUP_REMOVE_USER,
        DELETE,
        NEW_FOLDER
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
