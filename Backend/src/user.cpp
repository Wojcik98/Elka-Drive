#include "include/user.h"

/*!
 * \brief Konstruktor.
 * \param name Nazwa użytkonwika
 * \param id ID użytkownika
 */
User::User(const QString &name, const int id) : name(name), id(id) {

}

/*!
 * \brief Konstruktor kopiujący.
 * \param other Użytkownik, którego kopię tworzymy.
 */
User::User(const User &other) {
    this->id = other.id;
    this->name = other.name;
}

/*!
 * \brief Tworzy użytkownika na podstawie formatu JSON.
 * \param json Dane do przetworzenia
 * \return Użytkownik z JSON
 */
User User::fromJson(const QJsonObject &json) {
    QString name = json["username"].toString();
    int id = json["id"].toInt();

    return User(name, id);
}

/*!
 * \return Nazwa użytkownika
 */
QString User::getName() {
    return name;
}

/*!
 * \return ID użytkownika
 */
int User::getId() {
    return id;
}
