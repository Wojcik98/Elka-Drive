#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QString>

class Message {
public:
    Message(QString user, QString text, QDateTime timestamp)
        : user(user), text(text), timestamp(timestamp) {}

    QString getUser() {
        return user;
    }

    QString getMsg() {
        return text;
    }

private:
    QString user;
    QString text;
    QDateTime timestamp;
};

#endif // MESSAGE_H
