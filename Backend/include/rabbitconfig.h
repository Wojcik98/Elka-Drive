#ifndef RABBITCONFIG_H
#define RABBITCONFIG_H

#include <QString>

class RabbitConfig {
public:
    RabbitConfig(QString host, QString vhost, QString username, QString password, quint16 port)
        : host(host), vhost(vhost), username(username), password(password), port(port) {

    }

    QString getHost() {
        return host;
    }

    QString getVhost() {
        return vhost;
    }

    QString getUsername() {
        return username;
    }

    QString getPassword() {
        return password;
    }

    quint16 getPort() {
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
