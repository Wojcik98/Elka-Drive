#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"
#include "include/user.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model(APIBridge *bridge);

    bool isLogged();

    void requestLogin(QString user, QString password);
    void requestRegister(QString user, QString password);
    void requestDelete(QString path);
    void requestDownload(QString path);
    void requestPath(QString path);
    void requestGroups();
    void requestNewGroup(QString groupName);
    void requestGroupUsers(int groupId);

    static const int TYPE_ROLE = Qt::UserRole + 1;
    static const int ID_ROLE = Qt::UserRole + 2;

private:
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

    APIBridge *bridge;
    bool logged = false;

signals:
    void loginStatus(bool success);
    void registerStatus(bool success);
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode(int statusCode);
    void pathReceived(QList<QStandardItem*> groups);
    void groupUsersReceived(QList<User> users);
    void groupDeletedReceived();
    void groupAddUserReceived(bool success);
    void groupRemoveUserReceived(bool success);

public slots:
    void gotResponse(Response response);

    void requestGroupDelete(int groupId);
    void requestAddUserToGroup(QString username, int groupId);
    void requestRemoveUserFromGroup(QString username, int groupId);
};

#endif // MODEL_H
