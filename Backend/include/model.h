#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"
#include "include/user.h"

class Model : public QObject {
    Q_OBJECT

public:
    enum NewGroupStatus {
        OK,
        NAME_EXIST,
        SERVER_ERROR
    };

    Model(APIBridge *bridge);

    bool isLogged();
    void setLogged(bool logged);

    void requestDelete(QString path);
    void requestDownload(QString path);
    void requestPath(QString path);
    void requestGroups();
    void requestNewGroup(QString groupName);
    void requestGroupUsers(int groupId);

    static const int TYPE_ROLE = Qt::UserRole + 1;
    static const int ID_ROLE = Qt::UserRole + 2;

private:
    static const int STATUS_UNAUTHORIZED = 401;
    static const int STATUS_FORBIDDEN = 403;
    static const int STATUS_FOUND = 302;
    static const int STATUS_OK = 200;

    APIBridge *bridge;
    bool logged = false;

    void handleLoginResponse(Response response);
    void handleRegisterResponse(Response response);
    void handleGroupsResponse(Response response);
    void handleNewGroupResponse(Response response);
    void handlePathResponse(Response response);
    void handleFileResponse(Response response);
    void handleGroupUsersResponse(Response response);
    void handleGroupDeleteResponse(Response response);
    void handleGroupAddUserResponse(Response response);
    void handleGroupRemoveUserResponse(Response response);

    QList<QStandardItem*> parseDirectory(QByteArray json);

signals:
    void unauthorized();
    void loginStatus(bool success);
    void registerStatus(bool success);
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode(int statusCode);
    void pathReceived(QList<QStandardItem*> groups, bool forbidden);
    void groupUsersReceived(QList<User> users, bool forbidden);
    void groupDeletedReceived(bool forbidden);
    void groupAddUserReceived(bool success, bool forbidden);
    void groupRemoveUserReceived(bool success, bool forbidden);
    void responseError(QNetworkReply::NetworkError error);

public slots:
    void gotResponse(Response response);

    void requestRegister(QString user, QString password);
    void requestLogin(QString user, QString password);
    void requestGroupDelete(int groupId);
    void requestAddUserToGroup(QString username, int groupId);
    void requestRemoveUserFromGroup(QString username, int groupId);
};

#endif // MODEL_H
