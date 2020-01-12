#include <exception>
#include <QtDebug>
#include <QtGlobal>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

#include "include/model.h"

Model::Model(APIBridge *bridge, MessageReceiver *receiver)
    : bridge(bridge), receiver(receiver) {
    receiver->start();
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
        &Model::handleResponseError
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
    connect(
        receiver,
        &MessageReceiver::messageReceived,
        this,
        &Model::gotMessage
    );
}

void Model::clearPath() {
    path.clear();
}

void Model::clearMsgs() {
    for (auto model : messages.values()) {
        model->deleteLater();
    }
    messages.clear();
}

void Model::requestGroups() {
    if (!pathRequestInProgress) {
        pathRequestInProgress = true;
        clearPath();
        bridge->requestGroups();
    }
}

void Model::requestNewGroup(const QString &groupName) {
    bridge->requestNewGroup(groupName);
}

void Model::requestSubpath(const QModelIndex &index) {
    if (!pathRequestInProgress) {
        QString subdir;
        auto type = index.data(Model::TYPE_ROLE).toInt();

        if (type == ItemType::GROUP) {
            subdir = index.data(Model::ID_ROLE).toString();
        } else if (type == ItemType::FOLDER) {
            subdir = index.data(Qt::DisplayRole).toString();
        } else {
            return;
        }

        path.append(subdir);
        pathRequestInProgress = true;
        bridge->requestPath(path.join("/"));
    }
}

void Model::goBack() {
    if (!pathRequestInProgress) {
        pathRequestInProgress = true;

        if (path.length() > 1) {
            path.pop_back();
            bridge->requestPath(path.join("/"));
        } else if (path.length() == 1) {
            path.pop_back();
            bridge->requestGroups();
        }
    }
}

void Model::refresh() {
    if (!pathRequestInProgress) {
        pathRequestInProgress = true;

        if (path.length() > 0) {
            bridge->requestPath(path.join("/"));
        } else {
            bridge->requestGroups();
        }
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

void Model::requestDownload(const QModelIndex &index, const QString &path) {
    auto id = index.data(ID_ROLE).toInt();
    int type = index.data(TYPE_ROLE).toInt();

    if (type == ItemType::FILE) {
        bridge->requestFileDownload(id, path);
    } else if (type == ItemType::FOLDER) {
        bridge->requestDirectoryDownload(id, path);
    }
}

void Model::requestLogin(const QString &user, const QString &password) {
    usernameTrying = user;
    bridge->requestLogin(user, password);
}

void Model::requestRegister(const QString &user, const QString &password) {
    bridge->requestRegister(user, password);
}

void Model::requestGroupUsers(int groupId) {
    bridge->requestGroupUsers(groupId);
}

void Model::requestGroupDelete(int groupId) {
    bridge->requestGroupDelete(groupId);
}

void Model::requestAddUserToGroup(const QString &username, const int groupId) {
    bridge->requestAddUserToGroup(username, groupId);
}

void Model::requestRemoveUserFromGroup(const QString &username, const int groupId) {
    bridge->requestRemoveUserFromGroup(username, groupId);
}

void Model::requestNewFolder(const QString &name) {
    bridge->requestNewFolder(path.join("/") + "/" + name);
}

void Model::requestFileUpload(const QString &rootLocal, const QString &relativePath) {
    bridge->requestFileUpload(rootLocal, this->path.join("/"), relativePath);
}

void Model::sendMsg(const QString &msg) {
    if (path.length() > 0) {
        auto groupId = path.first().toInt();
        bridge->sendMsg(groupId, msg);
    }
}

bool Model::isLogged() {
    return logged;
}

void Model::setLogged(bool logged) {
    this->logged = logged;
}

void Model::gotResponse(const Response &response) {
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
        case RequestType::SEND_MSG:
            // do nothing
            break;
    }
}

void Model::handleLoginResponse(const Response&) {
    logged = true;
    username = usernameTrying;
    receiver->setUser(username);
    emit userLogged();
}

void Model::handleRegisterResponse(const Response&) {
    emit userRegistered();
}

void Model::handleGroupsResponse(const Response &response) {
    pathRequestInProgress = false;

    QList<int> groupIds;
    QList<QStandardItem*> groups;
    auto groupsRaw = QJsonDocument::fromJson(response.getBody());

    auto array = groupsRaw.array();

    for (auto groupRaw : array) {
        QJsonObject obj = groupRaw.toObject();
        QString name = obj["name"].toString();
        auto id = obj["id"].toInt();
        QStandardItem *group = new QStandardItem(QIcon(":/icons/folder_shared.svg"), name);
        group->setData(QVariant(ItemType::GROUP), TYPE_ROLE);
        group->setData(QVariant(QString::number(id)), ID_ROLE);
        groups.append(group);
        groupIds.append(id);
        groupIdToName[id] = name;
    }

    receiver->connectOnlyGivenGroups(groupIds);

    auto existingGroups = messages.keys();
    for (auto group : existingGroups) {
        if (!groupIds.contains(group)) {
            messages.remove(group);
        }
    }

    emit groupsReceived(groups);
}

void Model::handleNewGroupResponse(const Response&) {
    emit newGroupStatusCode();
}

void Model::handlePathResponse(const Response &response) {
    pathRequestInProgress = false;

    QList<QStandardItem*> dir = parseDirectory(response.getBody());

    emit pathReceived(dir);
}

QList<QStandardItem*> Model::parseDirectory(const QByteArray &json) {
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

void Model::handleGroupUsersResponse(const Response &response) {
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

void Model::handleGroupDeleteResponse(const Response&) {
    emit groupDeletedReceived();
}

void Model::handleGroupAddUserResponse(const Response&) {
    emit groupAddUserReceived();
}

void Model::handleGroupRemoveUserResponse(const Response&) {
    emit groupRemoveUserReceived();
}

void Model::handleDeleteResponse(const Response&) {
    emit resourceDeleted();
}

void Model::handleNewFolderResponse(const Response&) {
    emit newFolderCreated();
}

void Model::handleUploadResponse(const Response&) {
    emit uploadComplete();
}

void Model::gotMessage(const int groupId, const Message &msg) {
    QString text = msg.getMsg();
    auto item = new QStandardItem(text);
    item->setData(QVariant(msg.getUser()), Message::Role::USER);
    item->setData(QVariant(msg.getTimestamp()), Message::Role::TIMESTAMP);

    if (!messages.contains(groupId)) {
        messages[groupId] = new QStandardItemModel(this);
    }
    messages[groupId]->appendRow(item);
}

QStandardItemModel *Model::getCurrentGroupMessages() {
    if (path.size() > 0) {
        auto groupId = path.first().toInt();
        if (!messages.contains(groupId)) {
            messages[groupId] = new QStandardItemModel(this);
        }

        return messages[groupId];
    } else {
        return nullptr;
    }
}

void Model::handleResponseError(const QNetworkReply::NetworkError &error, const Response &response) {
    auto pathResponses = QList<RequestType>({RequestType::PATH, RequestType::GROUPS});
    if (pathResponses.contains(response.getType())) {
        pathRequestInProgress = false;
    }

    emit responseError(error, response);
}

QString Model::getPath() {
    if (path.size() > 0) {
        auto copy(path);
        auto id = copy.takeFirst().toInt();
        copy.prepend(groupIdToName[id]);

        return copy.join("/");
    } else {
        return "Groups";
    }
}
