#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>

class Message {
public:
    Message(QString user, QString text) : user(user), text(text) {}

    QString getUser() {
        return user;
    }

    QString getMsg() {
        return text;
    }

private:
    QString user;
    QString text;
};

#endif // MESSAGE_H
