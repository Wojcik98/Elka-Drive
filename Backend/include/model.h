#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include <QStandardItem>
#include "include/apibridge.h"
#include "include/messagereceiver.h"
#include "include/user.h"
#include "include/response.h"
#include "include/message.h"

class APIBridge;
class Response;

class Model : public QObject {
    Q_OBJECT

public:
    enum ItemType {
        GROUP,
        FOLDER,
        FILE
    };
    enum NewGroupStatus {
        OK,
        NAME_EXIST,
        SERVER_ERROR
    };

    Model(APIBridge *bridge, MessageReceiver *receiver);

    bool isLogged();
    void setLogged(bool logged);
    void clearPath();

    void requestDelete(const QModelIndex &index);
    void requestDownload(const QModelIndex &index, QString path);
    void requestSubpath(const QModelIndex &index);
    void requestGroups();
    void requestNewGroup(QString groupName);
    void requestGroupUsers(int groupId);
    void requestNewFolder(QString name);
    void requestFileUpload(QString rootLocal, QString relativePath);
    void goBack();
    void refresh();
    void sendMsg(const QString &msg);
    QStandardItemModel *getCurrentGroupMessages();

    static const int TYPE_ROLE = Qt::UserRole + 1;
    static const int ID_ROLE = Qt::UserRole + 2;

private:
    static const int STATUS_UNAUTHORIZED = 401;
    static const int STATUS_FORBIDDEN = 403;
    static const int STATUS_NOT_FOUND = 404;
    static const int STATUS_FOUND = 302;
    static const int STATUS_OK = 200;

    APIBridge *bridge;
    MessageReceiver *receiver;
    QStringList path;
    bool logged = false;
    QString usernameTrying;
    QString username;
    QMap<int, QStandardItemModel*> messages;

    void handleLoginResponse(Response response);
    void handleRegisterResponse(Response response);
    void handleGroupsResponse(Response response);
    void handleNewGroupResponse(Response response);
    void handlePathResponse(Response response);
    void handleGroupUsersResponse(Response response);
    void handleGroupDeleteResponse(Response response);
    void handleGroupAddUserResponse(Response response);
    void handleGroupRemoveUserResponse(Response response);
    void handleDeleteResponse(Response response);
    void handleNewFolderResponse(Response response);
    void handleUploadResponse(Response response);

    QList<QStandardItem*> parseDirectory(QByteArray json);

signals:
    void responseError(QNetworkReply::NetworkError error, Response requestType);
    void userLogged();
    void userRegistered();
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode();
    void pathReceived(QList<QStandardItem*> groups);
    void groupUsersReceived(QList<User> users);
    void groupDeletedReceived();
    void groupAddUserReceived();
    void groupRemoveUserReceived();
    void resourceDeleted();
    void newFolderCreated();
    void downloadProgress(qint64 current, qint64 total);
    void uploadProgress(qint64 current, qint64 total);
    void uploadComplete();
    void fileOpenError(const QString &filename);

public slots:
    void gotResponse(Response response);
    void gotMessage(int groupId, Message msg);

    void requestRegister(QString user, QString password);
    void requestLogin(QString user, QString password);
    void requestGroupDelete(int groupId);
    void requestAddUserToGroup(QString username, int groupId);
    void requestRemoveUserFromGroup(QString username, int groupId);
};

#endif // MODEL_H
