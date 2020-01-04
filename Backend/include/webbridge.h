#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "include/apibridge.h"

class WebBridge : public APIBridge {
public:
    WebBridge(QString mainUrl);
    virtual void requestLogin(QString user, QString password);
    virtual void requestRegister(QString user, QString password);
    virtual void requestGroups();
    virtual void requestNewGroup(QString groupName);
    virtual void requestPath(QString path);
    virtual void requestFileDownload(QString id);
    virtual void requestGroupUsers(int groupId);
    virtual void requestGroupDelete(int groupId);
    virtual void requestAddUserToGroup(QString username, int groupId);
    virtual void requestRemoveUserFromGroup(QString username, int groupId);
    virtual void requestFileDelete(QString id);
    virtual void requestDirectoryDelete(QString path);
    virtual void requestNewFolder(QString path);

private:
    static const int PROTOCOL_ERROR_HIGH = 100;

    QString mainUrl;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    Response::Type requestType;
    QByteArray dataRead;

public slots:
    void networkReplyReady();
    void networkReplyFinished();
};

#endif // WEBBRIDGE_H
