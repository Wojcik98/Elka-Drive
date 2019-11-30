#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include "include/apibridge.h"

class WebBridge : public APIBridge
{
public:
    WebBridge();
    virtual QJsonObject requestLogin(QString user, QString password);
};

#endif // WEBBRIDGE_H
