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
    virtual void requestNewGroup(QString groupName);
    virtual void requestPath(QString path);
    virtual void requestDownload(QString id);
    virtual void requestGroupUsers(int groupId);
    virtual void requestGroupDelete(int groupId);
    virtual void requestAddUserToGroup(QString username, int groupId);
    virtual void requestRemoveUserFromGroup(QString username, int groupId);

    void setResponse(Response resopnse);

private:
    Response response;
};

#endif // MOCKBRIDGE_H
