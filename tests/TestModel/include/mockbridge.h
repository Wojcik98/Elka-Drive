#ifndef MOCKBRIDGE_H
#define MOCKBRIDGE_H

#include "include/response.h"
#include "include/apibridge.h"

class MockBridge : public APIBridge {
public:
    MockBridge();
    void requestLogin(const QString &user, const QString &password) override;
    void requestRegister(const QString &user, const QString &password) override;
    void requestGroups() override;
    void requestNewGroup(const QString &groupName) override;
    void requestPath(const QString &path) override;
    void requestFileDownload(const int id, const QString &path) override;
    void requestDirectoryDownload(const int id, const QString &path) override;
    void requestGroupUsers(const int groupId) override;
    void requestGroupDelete(const int groupId) override;
    void requestAddUserToGroup(const QString &username, const int groupId) override;
    void requestRemoveUserFromGroup(const QString &username, const int groupId) override;
    void requestFileDelete(const int id) override;
    void requestDirectoryDelete(const QString &path) override;
    void requestNewFolder(const QString &path) override;
    void requestFileUpload(const QString &rootLocal, const QString &rootServer, const QString &path) override;
    void sendMsg(const int groupId, const QString &msg) override;

    void emitGotResponse(const Response &response);
    void emitResponseError(const QNetworkReply::NetworkError &error, const Response &response);
    void emitDownloadProgress(const qint64 bytesReceived, const qint64 bytesTotal);
    void emitUploadProgress(const qint64 bytesSent, const qint64 bytesTotal);
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
