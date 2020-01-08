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
    virtual void requestFileDownload(int id, QString path);
    virtual void requestDirectoryDownload(int id, QString path);
    virtual void requestGroupUsers(int groupId);
    virtual void requestGroupDelete(int groupId);
    virtual void requestAddUserToGroup(QString username, int groupId);
    virtual void requestRemoveUserFromGroup(QString username, int groupId);
    virtual void requestFileDelete(int id);
    virtual void requestDirectoryDelete(QString path);
    virtual void requestNewFolder(QString path);
    virtual void requestFileUpload(QString rootLocal, QString rootServer, QString path);
    virtual void sendMsg(int groupId, const QString &msg);

    void emitGotResponse(Response response);
    void emitResponseError(QNetworkReply::NetworkError error, Response response);
    void emitDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void emitUploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void emitFileOpenError(const QString &filename);

private:
    enum Action {
        GOT_RESPONSE,
        RESPONSE_ERROR,
        DOWNLOAD_PROGRESS,
        UPLOAD_PROGRESS,
        FILE_OPEN_ERROR
    };

    Action action;
    Response response;
    QNetworkReply::NetworkError error;
    qint64 current, total;
    QString filename;

    void takeAction();
};

#endif // MOCKBRIDGE_H
