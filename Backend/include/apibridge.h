#ifndef APIBRIDGE_H
#define APIBRIDGE_H

#include <QNetworkReply>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include "include/response.h"

class Response;

class APIBridge : public QObject {
Q_OBJECT

public:
    APIBridge();
    virtual ~APIBridge();
    virtual void requestLogin(const QString &user, const QString &password) = 0;
    virtual void requestRegister(const QString &user, const QString &password) = 0;
    virtual void requestGroups() = 0;
    virtual void requestNewGroup(const QString &groupName) = 0;
    virtual void requestPath(const QString &path) = 0;
    virtual void requestFileDownload(const int id, const QString &path) = 0;
    virtual void requestDirectoryDownload(const int id, const QString &path) = 0;
    virtual void requestGroupUsers(const int groupId) = 0;
    virtual void requestGroupDelete(const int groupId) = 0;
    virtual void requestAddUserToGroup(const QString &username, const int groupId) = 0;
    virtual void requestRemoveUserFromGroup(const QString &username, const int groupId) = 0;
    virtual void requestFileDelete(const int id) = 0;
    virtual void requestDirectoryDelete(const QString &path) = 0;
    virtual void requestNewFolder(const QString &path) = 0;
    virtual void requestFileUpload(const QString &rootLocal, const QString &rootServer, const QString &path) = 0;
    virtual void sendMsg(const int groupId, const QString &msg) = 0;

signals:
    void responseError(const QNetworkReply::NetworkError &error, const Response &requestType);
    void gotResponse(const Response &response);
    void downloadProgress(const qint64 bytesReceived, const qint64 bytesTotal);
    void uploadProgress(const qint64 bytesSent, const qint64 bytesTotal);
    void fileOpenError(const QString &filename);
};

#endif // APIBRIDGE_H
