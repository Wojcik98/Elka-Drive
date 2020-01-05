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

class WebBridge : public APIBridge {
public:
    WebBridge(QString mainUrl);
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

private:
    void get(QUrl url, Response::Type type);
    void post(QUrl url, QUrlQuery data, Response::Type type);
    void deleteResource(QUrl url, Response::Type type);
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
    void uploadReplyFinished();
};

#endif // WEBBRIDGE_H
