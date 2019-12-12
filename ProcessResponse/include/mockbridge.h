#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "include/apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    virtual void requestLogin(QString user, QString password);

    void setResponse(QJsonObject response);

private:
    QJsonObject response;
};

#endif // MOCKBRIDGE_H
