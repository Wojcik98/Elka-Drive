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
}

void Model::clearPath() {
    path.clear();
}

void Model::requestGroups() {
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

bool Model::isLogged() {
    return logged;
}

void Model::setLogged(bool logged) {
    this->logged = logged;
}

void Model::gotResponse(Response response) {
    // TODO move to bridge?
    if (response.getStatus() == STATUS_UNAUTHORIZED &&
        response.getType() != Response::Type::LOGIN) {
        emit unauthorized();
        return;
    }

    switch (response.getType()) {
        case Response::Type::LOGIN:
            handleLoginResponse(response);
            break;
        case Response::Type::REGISTER:
            handleRegisterResponse(response);
            break;
        case Response::Type::GROUPS:
            handleGroupsResponse(response);
            break;
        case Response::Type::NEW_GROUP:
            handleNewGroupResponse(response);
            break;
        case Response::Type::PATH:
            handlePathResponse(response);
            break;
        case Response::Type::GROUP_USERS:
            handleGroupUsersResponse(response);
            break;
        case Response::Type::GROUP_DELETE:
            handleGroupDeleteResponse(response);
            break;
        case Response::Type::GROUP_ADD_USER:
            handleGroupAddUserResponse(response);
            break;
        case Response::Type::GROUP_REMOVE_USER:
            handleGroupRemoveUserResponse(response);
            break;
        case Response::Type::DELETE:
            handleDeleteResponse(response);
            break;
        case Response::Type::NEW_FOLDER:
            handleNewFolderResponse(response);
            break;
    }
}

void Model::handleLoginResponse(Response response) {
    bool success = response.getStatus() == STATUS_FOUND;
    logged = success;
    emit loginStatus(success);
}

void Model::handleRegisterResponse(Response response) {
    bool success = response.getStatus() == STATUS_OK;
    emit registerStatus(success);
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

void Model::handleNewGroupResponse(Response response) {
    int statusCode = response.getStatus();
    // TODO handle here, not in controller
    emit newGroupStatusCode(statusCode);
}

void Model::handlePathResponse(Response response) {
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;
    QList<QStandardItem*> dir;

    if (!forbidden) {
        dir = parseDirectory(response.getBody());
    }

    emit pathReceived(dir, forbidden);
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
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;

    if (!forbidden) {
        auto respRaw = QJsonDocument::fromJson(response.getBody()).object();
        auto array = respRaw["users"].toArray();

        for (auto groupRaw : array) {
            QJsonObject json = groupRaw.toObject();
            auto user = User::fromJson(json);
            users.append(user);
        }
    }

    emit groupUsersReceived(users, forbidden);
}

void Model::handleGroupDeleteResponse(Response response) {
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;

    emit groupDeletedReceived(forbidden);
}

void Model::handleGroupAddUserResponse(Response response) {
    bool success = response.getStatus() == STATUS_OK;
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;

    emit groupAddUserReceived(success, forbidden);
}

void Model::handleGroupRemoveUserResponse(Response response) {
    bool success = response.getStatus() == STATUS_OK;
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;

    emit groupRemoveUserReceived(success, forbidden);
}

void Model::handleDeleteResponse(Response response) {
    bool success = response.getStatus() == STATUS_OK;
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;
    bool notFound = response.getStatus() == STATUS_NOT_FOUND;

    emit resourceDeleted(success, notFound, forbidden);
}

void Model::handleNewFolderResponse(Response response) {
    bool success = response.getStatus() == STATUS_OK;
    bool forbidden = response.getStatus() == STATUS_FORBIDDEN;

    emit newFolderCreated(success, forbidden);
}

void Model::requestFileUpload(QString rootLocal, QString relativePath) {
    bridge->requestFileUpload(rootLocal, this->path.join("/"), relativePath);
}
