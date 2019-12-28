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
}

void Model::requestGroups() {
    bridge->requestGroups();
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

bool Model::isLogged() {
    return logged;
}

void Model::gotResponse(Response response) {
    switch (response.getType()) {
        case Response::Type::LOGIN:
            handleLoginResponse(response);
            break;
        case Response::Type::REGISTER:
            break;
        case Response::Type::GROUPS:
            handleGroupsResponse(response);
            break;
        case Response::Type::PATH:
            handlePathResponse(response);
            break;
        case Response::Type::FILE:
            handleFileResponse(response);
            break;
    }
}

void Model::handleLoginResponse(Response response) {
    const int STATUS_OK = 302;
    bool success = response.getStatus() == STATUS_OK;
    logged = success;
    emit loginStatus(success);
}

void Model::handleGroupsResponse(Response response) {
    const int STATUS_OK = 200;
    if (response.getStatus() != STATUS_OK) {
        qDebug() << "Groups response not ok";
        return;
    }

    QList<QStandardItem*> groups;
    auto groupsRaw = QJsonDocument::fromJson(response.getBody());

    auto array = groupsRaw.array();

    for (auto groupRaw : array) {
        QJsonObject obj = groupRaw.toObject();
        QString name = obj["name"].toString();
        auto id = QString::number(obj["id"].toInt());
        qDebug() << id;
        QStandardItem *group = new QStandardItem(QIcon(":/icons/group.png"), name);
        group->setData(QVariant("group"));
        group->setData(QVariant(id), ID_ROLE);
        groups.append(group);
    }

    emit groupsReceived(groups);
}

void Model::handlePathResponse(Response response) {
    const int STATUS_OK = 200;
    if (response.getStatus() != STATUS_OK) {
        qDebug() << "Path response not ok";
        return;
    }

    QList<QStandardItem*> dir;
    auto dirRaw = QJsonDocument::fromJson(response.getBody());

    auto array = dirRaw.array();

    for (auto groupRaw : array) {
        QJsonObject obj = groupRaw.toObject();
        QString name = obj["name"].toString();
        auto id = QString::number(obj["id"].toInt());
        auto isDir = obj["dir"].toBool();

        QStandardItem *current;
        if (isDir) {
            current = new QStandardItem(QIcon(":/icons/folder.png"), name);
            current->setData(QVariant("folder"));
        } else {
            current = new QStandardItem(QIcon(":/icons/misc.png"), name);
            current->setData(QVariant("file"));
        }
        current->setData(QVariant(id), ID_ROLE);
        dir.append(current);
    }

    emit pathReceived(dir);
}

void Model::handleFileResponse(Response response) {
    const int STATUS_OK = 200;
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
