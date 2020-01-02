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
}

void Model::requestGroups() {
    bridge->requestGroups();
}

void Model::requestNewGroup(QString groupName) {
    bridge->requestNewGroup(groupName);
}

void Model::requestPath(QString path) {
    bridge->requestPath(path);
}

void Model::requestDelete(QString id) {
    qDebug() << "Delete!";
}

void Model::requestDownload(QString id) {
    bridge->requestDownload(id);
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

bool Model::isLogged() {
    return logged;
}

void Model::setLogged(bool logged) {
    this->logged = logged;
}

void Model::gotResponse(Response response) {
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
        case Response::Type::FILE:
            handleFileResponse(response);
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
        group->setData(QVariant("group"));
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
            current->setData(QVariant("folder"));
        } else {
            current = new QStandardItem(QIcon(":/icons/file.svg"), name);
            current->setData(QVariant("file"));
        }
        current->setData(QVariant(id), ID_ROLE);
        result.append(current);
    }

    return result;
}

void Model::handleFileResponse(Response response) {
    // TODO handle forbidden
    if (response.getStatus() != STATUS_OK) {
        qDebug() << "Download response not ok";
        return;
    }

    auto content = response.getBody();
    QString rawName = response.getName();
    qDebug() << "Downloaded raw:" << rawName;
    QString name = rawName
            .right(rawName.size() - rawName.indexOf('=') - 2);
    name.chop(1);
    qDebug() << "Downloaded:" << name;

    QFile file(name);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error opening file!";
    }

    QTextStream stream(&file);
    stream << content;

    file.close();
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
