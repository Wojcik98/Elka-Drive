#include "include/user.h"

User::User(const QString &name, const int id) : name(name), id(id) {

}

User::User(const User &other) {
    this->id = other.id;
    this->name = other.name;
}

User User::fromJson(const QJsonObject &json) {
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
