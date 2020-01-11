#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QString>

class Message {
public:
    enum Role {
        TEXT = Qt::DisplayRole,
        USER = Qt::UserRole + 1,
        TIMESTAMP = Qt::UserRole + 2
    };

    Message(QString user, QString text, QDateTime timestamp)
        : user(user), text(text), timestamp(timestamp) {}

    QString getUser() {
        return user;
    }

    QString getMsg() {
        return text;
    }

    QString getTimestamp() {
        return timestamp.toString("hh:mm:ss dd MMMM");
    }

private:
    QString user;
    QString text;
    QDateTime timestamp;
};

#endif // MESSAGE_H
