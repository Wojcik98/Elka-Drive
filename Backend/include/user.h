#ifndef USER_H
#define USER_H

#include <QString>
#include <QJsonObject>

/*!
 * \brief User przechowuje informacje o użytkowniku.
 */
class User {
public:
    User(const QString &name, const int id);
    User(const User &other);

    static User fromJson(const QJsonObject &json);

    QString getName();
    int getId();

private:
    QString name;
    int id;
};

#endif // USER_H
