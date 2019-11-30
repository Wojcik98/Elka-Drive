#ifndef APIBRIDGE_H
#define APIBRIDGE_H

#include <QJsonObject>
#include <QString>

class APIBridge {
public:
    APIBridge();
    virtual ~APIBridge();
    virtual QJsonObject requestLogin(QString user, QString password) = 0;
};

#endif // APIBRIDGE_H
