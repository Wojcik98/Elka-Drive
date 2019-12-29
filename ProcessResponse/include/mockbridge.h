#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "include/response.h"
#include "include/apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    virtual void requestLogin(QString user, QString password);
    virtual void requestRegister(QString user, QString password);
    virtual void requestGroups();
    virtual void requestPath(QString path);
    virtual void requestDownload(QString id);

    void setResponse(Response resopnse);

private:
    Response response;
};

#endif // MOCKBRIDGE_H
