#include <exception>
#include <QtDebug>
#include <QtGlobal>
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

QList<QStandardItem*> Model::getPath(QString path) {
    QList<QStandardItem*> result;

    for (int i = 0; i < qrand() % 5; ++i) {
        QStandardItem *folder = new QStandardItem(QIcon(":/icons/folder.png"), "Folder " + QString::number(i));
        folder->setData(QVariant("folder"));
        result.append(folder);
    }

    for (int i = 0; i < qrand() % 5; ++i) {
        QStandardItem *file = new QStandardItem(QIcon(":/icons/misc.png"), "Misc 4" + QString::number(i));
        file->setData(QVariant("misc"));
        result.append(file);
    }

    return result;
}

void Model::requestGroups() {
    bridge->requestGroups();
}

void Model::requestPath(QString path) {
}

void Model::requestDelete(QString path) {
    qDebug() << "Delete!";
}

void Model::requestDownload(QString path) {
    qDebug() << "Download!";
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
        QStandardItem *group = new QStandardItem(QIcon(":/icons/group.png"), name);
        group->setData(QVariant("group"));
        groups.append(group);
    }

    emit groupsReceived(groups);
}
