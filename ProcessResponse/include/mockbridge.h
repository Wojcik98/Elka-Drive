#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "include/response.h"
#include "include/apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    virtual void requestLogin(QString user, QString password);

    void setResponse(Response resopnse);

private:
    Response response;
};

#endif // MOCKBRIDGE_H
