#include "include/webbridge.h"
#include <QUrlQuery>
#include <QDebug>
#include <QNetworkReply>
#include <QHttpMultiPart>

WebBridge::WebBridge(QString mainUrl) : mainUrl(mainUrl) {
    requestReply = nullptr;
    currentRequest = nullptr;
    downloadReply = nullptr;
    currentDownload = nullptr;
    uploadReply = nullptr;
    currentUpload = nullptr;
}

void WebBridge::requestLogin(QString user, QString password) {
    auto url = QUrl(mainUrl + "/login");
    QUrlQuery data;
    data.addQueryItem("username", user);
    data.addQueryItem("password", password);

    post(url, data, Response::Type::LOGIN);
}

void WebBridge::requestRegister(QString user, QString password) {
    auto url = QUrl(mainUrl + "/register");
    QUrlQuery data;
    data.addQueryItem("username", user);
    data.addQueryItem("password", password);

    post(url, data, Response::Type::REGISTER);
}

void WebBridge::requestFileDelete(int id) {
    auto url = QUrl(mainUrl + "/files/" + QString::number(id));

    deleteResource(url, Response::Type::DELETE);
}

void WebBridge::requestDirectoryDelete(QString path) {
    auto url = QUrl(mainUrl + "/files/del");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    deleteResource(url, Response::Type::DELETE);
}

void WebBridge::requestGroups() {
    auto url = QUrl(mainUrl + "/groups/my");

    get(url, Response::Type::GROUPS);
}

void WebBridge::requestNewGroup(QString groupName) {
    auto url = QUrl(mainUrl + "/groups");
    QUrlQuery data;
    data.addQueryItem("name", groupName);

    post(url, data, Response::Type::NEW_GROUP);
}

void WebBridge::requestPath(QString path) {
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    get(url, Response::Type::PATH);
}

void WebBridge::requestGroupUsers(int groupId) {
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    get(url, Response::Type::GROUP_USERS);
}

void WebBridge::requestGroupDelete(int groupId) {
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    deleteResource(url, Response::Type::GROUP_DELETE);
}

void WebBridge::requestAddUserToGroup(QString username, int groupId) {
    auto url = QUrl(mainUrl + "/groups/add/" + QString::number(groupId));
    QUrlQuery data;
    data.addQueryItem("username", username);

    post(url, data, Response::Type::GROUP_ADD_USER);
}

void WebBridge::requestRemoveUserFromGroup(QString username, int groupId) {
    auto url = QUrl(mainUrl + "/groups/remove/" + QString::number(groupId));
    QUrlQuery data;
    data.addQueryItem("username", username);

    post(url, data, Response::Type::GROUP_REMOVE_USER);
}

void WebBridge::requestNewFolder(QString path) {
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery data;
    data.addQueryItem("path", path);

    post(url, data, Response::Type::NEW_FOLDER);
}

void WebBridge::get(QUrl url, Response::Type type) {
    auto request = QNetworkRequest(url);
    auto newRequest = new Request(Request::Method::GET, request, type);
    requestQueue.enqueue(newRequest);

    triggerRequest();
}

void WebBridge::post(QUrl url, QUrlQuery data, Response::Type type) {
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    auto newRequest = new Request(Request::Method::POST, request, type);
    newRequest->setData(data.toString(QUrl::FullyEncoded).toUtf8());
    requestQueue.enqueue(newRequest);

    triggerRequest();
}

void WebBridge::deleteResource(QUrl url, Response::Type type) {
    auto request = QNetworkRequest(url);
    auto newRequest = new Request(Request::Method::DELETE, request, type);
    requestQueue.enqueue(newRequest);

    triggerRequest();
}

void WebBridge::triggerRequest() {
    if (requestReply != nullptr || currentRequest != nullptr || requestQueue.empty()) {
        return;
    }

    currentRequest = requestQueue.dequeue();
    dataRead.clear();
    auto request = currentRequest->getRequest();

    switch (currentRequest->getMethod()) {
        case Request::Method::GET:
            requestReply = manager.get(request);
            break;
        case Request::Method::POST:
            requestReply = manager.post(request, currentRequest->getData());
            break;
        case Request::Method::DELETE:
            requestReply = manager.deleteResource(request);
            break;
    }

    connectReply();
}

void WebBridge::connectReply() {
    connect(
        requestReply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        requestReply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::networkReplyReady() {
    dataRead.append(requestReply->readAll());
}

void WebBridge::networkReplyFinished() {
    QVariant statusCode = requestReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    auto error = requestReply->error();
    if (error != QNetworkReply::NoError &&
        error < PROTOCOL_ERROR_HIGH) {
        qDebug() << "error: " << error;

        delete currentRequest;
        currentRequest = nullptr;
        requestReply->deleteLater();
        requestReply = nullptr;
        emit responseError(error);
    } else {
        auto type = currentRequest->getType();
        Response response(statusCode.toInt(), dataRead, type);

        delete currentRequest;
        currentRequest = nullptr;
        requestReply->deleteLater();
        requestReply = nullptr;
        emit gotResponse(response);
    }

    triggerRequest();
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
    if (downloadReply != nullptr || currentDownload != nullptr || downloadQueue.empty()) {
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
    downloadReply->deleteLater();
    downloadReply = nullptr;
    currentDownload->closeFile();
    delete currentDownload;
    currentDownload = nullptr;
    // TODO emit some signals
    triggerDownload();
}

void WebBridge::requestFileUpload(QString rootLocal, QString rootServer, QString path) {
    qDebug() << rootLocal << rootServer << path;
    auto newUpload = new UploadItem(rootLocal, rootServer, path);
    qDebug() << newUpload->getFilename() << newUpload->getRootLocal() << newUpload->getRootServer();
    uploadQueue.enqueue(newUpload);
    triggerUpload();
}

void WebBridge::triggerUpload() {
    if (uploadReply != nullptr || currentUpload != nullptr || uploadQueue.empty()) {
        return;
    }

    currentUpload = uploadQueue.dequeue();
    triggerUploadCreateDirectory();
}

void WebBridge::triggerUploadCreateDirectory() {
    // TODO implement
    triggerUploadSendFile();
}

void WebBridge::triggerUploadSendFile() {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart pathPart;
    pathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
    pathPart.setBody(currentUpload->getRootServer().toUtf8());

    QHttpPart filePart;
    auto header = "form-data; name=\"file\"; filename=\"" + currentUpload->getFilename() + "\"";
    filePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QVariant(header)
    );
    auto pathLocal = currentUpload->getRootLocal() + currentUpload->getRelativePath();
    QFile *file = new QFile(pathLocal);
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multiPart);

    multiPart->append(pathPart);
    multiPart->append(filePart);

    QUrl url(mainUrl + "/files");
    QNetworkRequest request(url);

    uploadReply = manager.post(request, multiPart);
    multiPart->setParent(uploadReply);

    connect(
        uploadReply,
        &QNetworkReply::finished,
        this,
        &WebBridge::uploadReplyFinished
    );
}

void WebBridge::uploadReplyFinished() {
    auto statusCode = uploadReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    auto response = uploadReply->readAll();
    qDebug() << "upload finished" << statusCode << response;

    delete currentUpload;
    currentUpload = nullptr;
    uploadReply->deleteLater();
    uploadReply = nullptr;
}
