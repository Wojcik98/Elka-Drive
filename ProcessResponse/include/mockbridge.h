#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "include/response.h"
#include "include/apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    void requestLogin(QString user, QString password) override;
    void requestRegister(QString user, QString password) override;
    void requestGroups() override;
    void requestNewGroup(QString groupName) override;
    void requestPath(QString path) override;
    void requestFileDownload(int id, QString path) override;
    void requestDirectoryDownload(int id, QString path) override;
    void requestGroupUsers(int groupId) override;
    void requestGroupDelete(int groupId) override;
    void requestAddUserToGroup(QString username, int groupId) override;
    void requestRemoveUserFromGroup(QString username, int groupId) override;
    void requestFileDelete(int id) override;
    void requestDirectoryDelete(QString path) override;
    void requestNewFolder(QString path) override;
    void requestFileUpload(QString rootLocal, QString rootServer, QString path) override;
    void sendMsg(int groupId, const QString &msg) override;

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
