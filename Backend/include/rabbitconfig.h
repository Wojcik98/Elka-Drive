#ifndef RABBITCONFIG_H
#define RABBITCONFIG_H

#include <QString>

class RabbitConfig {
public:
    RabbitConfig(const QString &host, const QString &vhost, const QString &username, const QString &password, const quint16 port)
        : host(host), vhost(vhost), username(username), password(password), port(port) {

    }

    QString getHost() const {
        return host;
    }

    QString getVhost() const {
        return vhost;
    }

    QString getUsername() const {
        return username;
    }

    QString getPassword() const {
        return password;
    }

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
