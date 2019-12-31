#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

class User {
public:
    User(QString name, int id);

    static User fromJson(QJsonObject json);

    QString getName();
    int getId();

private:
    QString name;
    int id;
};

#endif // USER_H
