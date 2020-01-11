#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QQueue>
#include "include/apibridge.h"
#include "include/downloaditem.h"
#include "include/uploaditem.h"
#include "include/request.h"
#include "include/requesttype.h"

class WebBridge : public APIBridge {
public:
    WebBridge(const QString &mainUrl);
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
    void requestFileUpload(const QString &rootLocal, const QString &rootServer, const QString &relativePath) override;
    void sendMsg(const int groupId, const QString &msg) override;

private:
    void get(const QUrl &url, const RequestType &type);
    void post(const QUrl &url, const QUrlQuery &data, const RequestType &type);
    void deleteResource(const QUrl &url, const RequestType &type);
    void connectReply();
    void triggerRequest();
    void requestDownload(const int id, const QString &path, const QUrl &url);
    void triggerDownload();
    void triggerUpload();
    void triggerUploadCreateDirectory();
    void triggerUploadSendFile();

    QString mainUrl;
    QNetworkAccessManager manager;
    QByteArray dataRead;

    QQueue<Request *> requestQueue;
    QNetworkReply *requestReply;
    Request *currentRequest;

    QQueue<DownloadItem *> downloadQueue;
    QNetworkReply *downloadReply;
    DownloadItem *currentDownload;

    QQueue<UploadItem *> uploadQueue;
    QNetworkReply *uploadReply;
    UploadItem *currentUpload;

private slots:
    void networkReplyReady();
    void networkReplyFinished();
    void downloadReplyReady();
    void downloadReplyFinished();
    void uploadCreateDirectoryFinished();
    void uploadSendFileFinished();
};

#endif // WEBBRIDGE_H
