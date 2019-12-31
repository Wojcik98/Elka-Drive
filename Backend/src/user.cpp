#include "include/user.h"

User::User(QString name, int id) : name(name), id(id) {

}

User User::fromJson(QJsonObject json) {
    QString name = json["username"].toString();
    int id = json["id"].toInt();

    return User(name, id);
}

QString User::getName() {
    return name;
}

int User::getId() {
    return id;
}
