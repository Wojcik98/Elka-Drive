#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    QJsonObject requestLogin(QString user, QString password);

    void setResponse(QJsonObject response);

private:
    QJsonObject response;
};

#endif // MOCKBRIDGE_H
