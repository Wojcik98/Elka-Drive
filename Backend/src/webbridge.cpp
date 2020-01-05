#include "include/webbridge.h"
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkReply>

WebBridge::WebBridge(QString mainUrl) : mainUrl(mainUrl) {
    reply = nullptr;
    downloadReply = nullptr;
}

void WebBridge::requestLogin(QString user, QString password) {
    if (reply != nullptr) {
        // TODO emit response with error code?
        qDebug() << "Another request in progress!";
        return;
    }
    auto url = QUrl(mainUrl + "/login");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", user);
    postData.addQueryItem("password", password);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::LOGIN;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestRegister(QString user, QString password) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    auto url = QUrl(mainUrl + "/register");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", user);
    postData.addQueryItem("password", password);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::REGISTER;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestFileDelete(int id) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }

    dataRead.clear();
    auto url = QUrl(mainUrl + "/files/" + QString::number(id));
    auto request = QNetworkRequest(url);

    requestType = Response::Type::DELETE;
    reply = manager.deleteResource(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestDirectoryDelete(QString path) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }

    dataRead.clear();
    auto url = QUrl(mainUrl + "/files/del");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    auto request = QNetworkRequest(url);
    requestType = Response::Type::DELETE;
    reply = manager.deleteResource(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestGroups() {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }

    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/my");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    requestType = Response::Type::GROUPS;
    reply = manager.get(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestNewGroup(QString groupName) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("name", groupName);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::NEW_GROUP;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestPath(QString path) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }

    dataRead.clear();
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    requestType = Response::Type::PATH;
    reply = manager.get(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestGroupUsers(int groupId) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    requestType = Response::Type::GROUP_USERS;
    reply = manager.get(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestGroupDelete(int groupId) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    requestType = Response::Type::GROUP_DELETE;
    reply = manager.deleteResource(request);

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestAddUserToGroup(QString username, int groupId) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/add/" + QString::number(groupId));
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::GROUP_ADD_USER;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestRemoveUserFromGroup(QString username, int groupId) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/remove/" + QString::number(groupId));
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", username);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::GROUP_REMOVE_USER;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::requestNewFolder(QString path) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }

    dataRead.clear();
    auto url = QUrl(mainUrl + "/files/dir");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("path", path);
    requestType = Response::Type::NEW_FOLDER;
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::networkReplyReady() {
    dataRead.append(reply->readAll());
}

void WebBridge::networkReplyFinished() {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    auto error = reply->error();
    if (error != QNetworkReply::NoError &&
        error < PROTOCOL_ERROR_HIGH) {
        qDebug() << "error: " << error;

        reply->deleteLater();
        reply = nullptr;
        emit responseError(error);
    } else {
        Response response(statusCode.toInt(), dataRead, requestType);

        reply->deleteLater();
        reply = nullptr;
        emit gotResponse(response);
    }
}

void WebBridge::requestFileDownload(int id, QString path) {
    auto url = QUrl(mainUrl + "/files/" + QString::number(id));
    requestDownload(id, path, url);
}

void WebBridge::requestDirectoryDownload(int id, QString path) {
    auto url = QUrl(mainUrl + "/files/dir/" + QString::number(id));
    requestDownload(id, path, url);
}

void WebBridge::requestDownload(int id, QString path, QUrl url) {
    auto newItem = new DownloadItem(id, path, url);
    downloadQueue.enqueue(newItem);
    triggerDownload();
}

void WebBridge::triggerDownload() {
    if (downloadReply != nullptr || downloadQueue.empty()) {
        return;
    }

    currentDownload = downloadQueue.dequeue();
    bool fileOpened = currentDownload->openFile();

    if (fileOpened) {
        auto request = QNetworkRequest(currentDownload->getUrl());
        downloadReply = manager.get(request);

        connect(
            downloadReply,
            &QNetworkReply::finished,
            this,
            &WebBridge::downloadReplyFinished
        );
        connect(
            downloadReply,
            &QIODevice::readyRead,
            this,
            &WebBridge::downloadReplyReady
        );
        // TODO progress
    } else {
        // TODO emit some error
        currentDownload = nullptr;
        triggerDownload();
    }
}

void WebBridge::downloadReplyReady() {
    currentDownload->appendData(downloadReply->readAll());
}

void WebBridge::downloadReplyFinished() {
    qDebug() << "download reply finished";
    // TODO emit some signals
    downloadReply->deleteLater();
    downloadReply = nullptr;
    currentDownload->closeFile();
    delete currentDownload;
    currentDownload = nullptr;
    triggerDownload();
}
