#include <exception>
#include <QtDebug>
#include <QtGlobal>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include "include/model.h"

Model::Model(APIBridge *bridge) : bridge(bridge) {
    connect(
        bridge,
        &APIBridge::gotResponse,
        this,
        &Model::gotResponse
    );
    connect(
        bridge,
        &APIBridge::responseError,
        this,
        &Model::responseError
    );
    connect(
        bridge,
        &APIBridge::downloadProgress,
        this,
        &Model::downloadProgress
    );
    connect(
        bridge,
        &APIBridge::uploadProgress,
        this,
        &Model::uploadProgress
    );
    connect(
        bridge,
        &APIBridge::fileOpenError,
        this,
        &Model::fileOpenError
    );
}

void Model::clearPath() {
    path.clear();
}

void Model::requestGroups() {
    // TODO block requesting several paths at once
    clearPath();
    bridge->requestGroups();
}

void Model::requestNewGroup(QString groupName) {
    bridge->requestNewGroup(groupName);
}

void Model::requestSubpath(const QModelIndex &index) {
    QString subdir;
    auto type = index.data(Model::TYPE_ROLE).toInt();

    if (type == ItemType::GROUP) {
        subdir = index.data(Model::ID_ROLE).toString();
    } else if (type == ItemType::FOLDER) {
        subdir = index.data(Qt::DisplayRole).toString();
    } else {
        return;
    }

    path.append(subdir); // TODO read path from response?
    bridge->requestPath(path.join("/"));
}

void Model::goBack() {
    if (path.length() > 1) {
        path.pop_back();
        bridge->requestPath(path.join("/"));
    } else if (path.length() == 1) {
        path.pop_back();
        bridge->requestGroups();
    }
}

void Model::refresh() {
    if (path.length() > 0) {
        bridge->requestPath(path.join("/"));
    } else {
        bridge->requestGroups();
    }
}

void Model::requestDelete(const QModelIndex &index) {
    auto id = index.data(Model::ID_ROLE).toInt();
    auto filename = index.data().toString();
    auto type = index.data(Model::TYPE_ROLE).toInt();

    if (type == ItemType::FOLDER) {
        bridge->requestDirectoryDelete(path.join("/") + "/" + filename);
    } else if (type == Model::ItemType::FILE) {
        bridge->requestFileDelete(id);
    }
}

void Model::requestDownload(const QModelIndex &index, QString path) {
    auto id = index.data(ID_ROLE).toInt();
    int type = index.data(TYPE_ROLE).toInt();

    if (type == ItemType::FILE) {
        bridge->requestFileDownload(id, path);
    } else if (type == ItemType::FOLDER) {
        bridge->requestDirectoryDownload(id, path);
    }
}

void Model::requestLogin(QString user, QString password) {
    bridge->requestLogin(user, password);
}

void Model::requestRegister(QString user, QString password) {
    bridge->requestRegister(user, password);
}

void Model::requestGroupUsers(int groupId) {
    bridge->requestGroupUsers(groupId);
}

void Model::requestGroupDelete(int groupId) {
    bridge->requestGroupDelete(groupId);
}

void Model::requestAddUserToGroup(QString username, int groupId) {
    bridge->requestAddUserToGroup(username, groupId);
}

void Model::requestRemoveUserFromGroup(QString username, int groupId) {
    bridge->requestRemoveUserFromGroup(username, groupId);
}

void Model::requestNewFolder(QString name) {
    bridge->requestNewFolder(path.join("/") + "/" + name);
}

void Model::requestFileUpload(QString rootLocal, QString relativePath) {
    bridge->requestFileUpload(rootLocal, this->path.join("/"), relativePath);
}

bool Model::isLogged() {
    return logged;
}

void Model::setLogged(bool logged) {
    this->logged = logged;
}

void Model::gotResponse(Response response) {
    switch (response.getType()) {
        case RequestType::LOGIN:
            handleLoginResponse(response);
            break;
        case RequestType::REGISTER:
            handleRegisterResponse(response);
            break;
        case RequestType::GROUPS:
            handleGroupsResponse(response);
            break;
        case RequestType::NEW_GROUP:
            handleNewGroupResponse(response);
            break;
        case RequestType::PATH:
            handlePathResponse(response);
            break;
        case RequestType::GROUP_USERS:
            handleGroupUsersResponse(response);
            break;
        case RequestType::GROUP_DELETE:
            handleGroupDeleteResponse(response);
            break;
        case RequestType::GROUP_ADD_USER:
            handleGroupAddUserResponse(response);
            break;
        case RequestType::GROUP_REMOVE_USER:
            handleGroupRemoveUserResponse(response);
            break;
        case RequestType::DELETE:
            handleDeleteResponse(response);
            break;
        case RequestType::NEW_FOLDER:
            handleNewFolderResponse(response);
            break;
        case RequestType::DOWNLOAD:
            // will not occur
            break;
        case RequestType::UPLOAD_NEW_FOLDER:
            // will not occur
            break;
        case RequestType::UPLOAD_SEND_FILE:
            handleUploadResponse(response);
            break;
    }
}

void Model::handleLoginResponse(Response) {
    logged = true;
    emit userLogged();
}

void Model::handleRegisterResponse(Response) {
    emit userRegistered();
}

void Model::handleGroupsResponse(Response response) {
    QList<QStandardItem*> groups;
    auto groupsRaw = QJsonDocument::fromJson(response.getBody());

    auto array = groupsRaw.array();

    for (auto groupRaw : array) {
        QJsonObject obj = groupRaw.toObject();
        QString name = obj["name"].toString();
        auto id = QString::number(obj["id"].toInt());
        QStandardItem *group = new QStandardItem(QIcon(":/icons/folder_shared.svg"), name);
        group->setData(QVariant(ItemType::GROUP), TYPE_ROLE);
        group->setData(QVariant(id), ID_ROLE);
        groups.append(group);
    }

    emit groupsReceived(groups);
}

void Model::handleNewGroupResponse(Response) {
    emit newGroupStatusCode();
}

void Model::handlePathResponse(Response response) {
    QList<QStandardItem*> dir = parseDirectory(response.getBody());

    emit pathReceived(dir);
}

QList<QStandardItem*> Model::parseDirectory(QByteArray json) {
    QList<QStandardItem*> result;

    auto dirRaw = QJsonDocument::fromJson(json);
    auto array = dirRaw.array();

    for (auto groupRaw : array) {
        QJsonObject obj = groupRaw.toObject();
        QString name = obj["name"].toString();
        auto id = QString::number(obj["id"].toInt());
        auto isDir = obj["dir"].toBool();

        QStandardItem *current;
        if (isDir) {
            current = new QStandardItem(QIcon(":/icons/folder.svg"), name);
            current->setData(QVariant(ItemType::FOLDER), TYPE_ROLE);
        } else {
            current = new QStandardItem(QIcon(":/icons/file.svg"), name);
            current->setData(QVariant(ItemType::FILE), TYPE_ROLE);
        }
        current->setData(QVariant(id), ID_ROLE);
        result.append(current);
    }

    return result;
}

void Model::handleGroupUsersResponse(Response response) {
    QList<User> users;

    auto respRaw = QJsonDocument::fromJson(response.getBody()).object();
    auto array = respRaw["users"].toArray();

    for (auto groupRaw : array) {
        QJsonObject json = groupRaw.toObject();
        auto user = User::fromJson(json);
        users.append(user);
    }

    emit groupUsersReceived(users);
}

void Model::handleGroupDeleteResponse(Response) {
    emit groupDeletedReceived();
}

void Model::handleGroupAddUserResponse(Response) {
    emit groupAddUserReceived();
}

void Model::handleGroupRemoveUserResponse(Response) {
    emit groupRemoveUserReceived();
}

void Model::handleDeleteResponse(Response) {
    emit resourceDeleted();
}

void Model::handleNewFolderResponse(Response) {
    emit newFolderCreated();
}

void Model::handleUploadResponse(Response) {
    emit uploadComplete();
}
