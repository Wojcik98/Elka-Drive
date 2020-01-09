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
    WebBridge(QString mainUrl);
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
    void requestFileUpload(QString rootLocal, QString rootServer, QString relativePath) override;
    void sendMsg(int groupId, const QString &msg) override;

private:
    void get(QUrl url, RequestType type);
    void post(QUrl url, QUrlQuery data, RequestType type);
    void deleteResource(QUrl url, RequestType type);
    void connectReply();
    void triggerRequest();
    void requestDownload(int id, QString path, QUrl url);
    void triggerDownload();
    void triggerUpload();
    void triggerUploadCreateDirectory();
    void triggerUploadSendFile();
    static const int PROTOCOL_ERROR_HIGH = 100;

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
