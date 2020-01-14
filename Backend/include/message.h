#ifndef MESSAGE_H
#define MESSAGE_H

#include <QDateTime>
#include <QString>

/*!
 * \brief Message przechowuje informacje o wiadomości z czatu.
 */
class Message {
public:
    /*!
     * \brief Role do wydobywania danych z QModelIndex
     */
    enum Role {
        TEXT = Qt::DisplayRole, /*!< Tekst wiadomości */
        USER = Qt::UserRole + 1, /*!< Użytkownik wysyłający */
        TIMESTAMP = Qt::UserRole + 2 /*!< Czas wysłania */
    };

    /*!
     * \brief Konstruktor.
     * \param user Użytkownik wysyłający
     * \param text Tekst wiadomości
     * \param timestamp Czas wysłania
     */
    Message(const QString &user, const QString &text, const QDateTime &timestamp)
        : user(user), text(text), timestamp(timestamp) {}

    /*!
     * \return Nazwa użytkownika
     */
    QString getUser() const {
        return user;
    }

    /*!
     * \return Tekst wiadomości
     */
    QString getMsg() const {
        return text;
    }

    /*!
     * \return Czas wysłania
     */
    QString getTimestamp() const {
        return timestamp.toString("hh:mm:ss dd MMMM");
    }

private:
    QString user;
    QString text;
    QDateTime timestamp;
};

#endif // MESSAGE_H
