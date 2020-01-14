#ifndef RABBITCONFIG_H
#define RABBITCONFIG_H

#include <QString>

/*!
 * \brief RabbitConfig przechowuje konfigurację do RabbitMQ.
 *
 * Przy korzystaniu z klasy należy uważać, aby dane logowania do RabbitMQ
 * nie wyciekły np. przez dodanie kodu inicjalizującego do repozytorium.
 */
class RabbitConfig {
public:
    /*!
     * \brief Konstruktor.
     * \param host Nazwa hosta
     * \param vhost Nazwa wirtualnego hosta
     * \param username Nazwa użytkownika
     * \param password Hasło
     * \param port Port RabbitMQ
     */
    RabbitConfig(const QString &host, const QString &vhost, const QString &username, const QString &password, const quint16 port)
        : host(host), vhost(vhost), username(username), password(password), port(port) {

    }

    /*!
     * \return Nazwa hosta
     */
    QString getHost() const {
        return host;
    }

    /*!
     * \return Nazwa wirtualnego hosta
     */
    QString getVhost() const {
        return vhost;
    }

    /*!
     * \return Nazwa użytkownika
     */
    QString getUsername() const {
        return username;
    }

    /*!
     * \return Hasło
     */
    QString getPassword() const {
        return password;
    }

    /*!
     * \return Port RabbitMQ
     */
    quint16 getPort() const {
        return port;
    }

private:
    QString host;
    QString vhost;
    QString username;
    QString password;
    quint16 port;
};

#endif // RABBITCONFIG_H
