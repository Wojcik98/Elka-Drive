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

    post(url, data, RequestType::LOGIN);
}

void WebBridge::requestRegister(QString user, QString password) {
    auto url = QUrl(mainUrl + "/register");
    QUrlQuery data;
    data.addQueryItem("username", user);
    data.addQueryItem("password", password);

    post(url, data, RequestType::REGISTER);
}

void WebBridge::requestFileDelete(int id) {
    auto url = QUrl(mainUrl + "/files/" + QString::number(id));

    deleteResource(url, RequestType::DELETE);
}

void WebBridge::requestDirectoryDelete(QString path) {
    auto url = QUrl(mainUrl + "/files/del");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    deleteResource(url, RequestType::DELETE);
}

void WebBridge::requestGroups() {
    auto url = QUrl(mainUrl + "/groups/my");

    get(url, RequestType::GROUPS);
}

void WebBridge::requestNewGroup(QString groupName) {
    auto url = QUrl(mainUrl + "/groups");
    QUrlQuery data;
    data.addQueryItem("name", groupName);

    post(url, data, RequestType::NEW_GROUP);
}

void WebBridge::requestPath(QString path) {
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery query;
    query.addQueryItem("path", path);
    url.setQuery(query.query());

    get(url, RequestType::PATH);
}

void WebBridge::requestGroupUsers(int groupId) {
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    get(url, RequestType::GROUP_USERS);
}

void WebBridge::requestGroupDelete(int groupId) {
    auto url = QUrl(mainUrl + "/groups/" + QString::number(groupId));

    deleteResource(url, RequestType::GROUP_DELETE);
}

void WebBridge::requestAddUserToGroup(QString username, int groupId) {
    auto url = QUrl(mainUrl + "/groups/add/" + QString::number(groupId));
    QUrlQuery data;
    data.addQueryItem("username", username);

    post(url, data, RequestType::GROUP_ADD_USER);
}

void WebBridge::requestRemoveUserFromGroup(QString username, int groupId) {
    auto url = QUrl(mainUrl + "/groups/remove/" + QString::number(groupId));
    QUrlQuery data;
    data.addQueryItem("username", username);

    post(url, data, RequestType::GROUP_REMOVE_USER);
}

void WebBridge::requestNewFolder(QString path) {
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery data;
    data.addQueryItem("path", path);

    post(url, data, RequestType::NEW_FOLDER);
}

void WebBridge::get(QUrl url, RequestType type) {
    auto request = QNetworkRequest(url);
    auto newRequest = new Request(Request::Method::GET, request, type);
    requestQueue.enqueue(newRequest);

    triggerRequest();
}

void WebBridge::post(QUrl url, QUrlQuery data, RequestType type) {
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    auto newRequest = new Request(Request::Method::POST, request, type);
    newRequest->setData(data.toString(QUrl::FullyEncoded).toUtf8());
    requestQueue.enqueue(newRequest);

    triggerRequest();
}

void WebBridge::deleteResource(QUrl url, RequestType type) {
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
    auto type = currentRequest->getType();
    Response response(dataRead, type);

    delete currentRequest;
    currentRequest = nullptr;
    requestReply->deleteLater();
    requestReply = nullptr;

    if (error != QNetworkReply::NoError) {
        qDebug() << "error: " << error;

        emit responseError(error, response);
    } else {
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
        connect(
            downloadReply,
            &QNetworkReply::downloadProgress,
            this,
            &WebBridge::downloadProgress
        );
    } else {
        emit fileOpenError(currentDownload->getFilePath());
        delete currentDownload;
        currentDownload = nullptr;
        triggerDownload();
    }
}

void WebBridge::downloadReplyReady() {
    currentDownload->appendData(downloadReply->readAll());
}

void WebBridge::downloadReplyFinished() {
    auto error = downloadReply->error();

    downloadReply->deleteLater();
    downloadReply = nullptr;
    currentDownload->closeFile();
    delete currentDownload;
    currentDownload = nullptr;

    if (error != QNetworkReply::NoError) {
        qDebug() << "error: " << error;

        auto response = Response("", RequestType::DOWNLOAD);
        emit responseError(error, response);
    }

    triggerDownload();
}

void WebBridge::requestFileUpload(QString rootLocal, QString rootServer, QString relativePath) {
    auto newUpload = new UploadItem(rootLocal, rootServer, relativePath);
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
    auto path = currentUpload->getRootServer() + "/" + currentUpload->getRelativeDir();
    auto url = QUrl(mainUrl + "/files/dir");
    QUrlQuery data;
    data.addQueryItem("path", path);
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    uploadReply = manager.post(request, data.toString(QUrl::FullyEncoded).toUtf8());
    connect(
        uploadReply,
        &QNetworkReply::finished,
        this,
        &WebBridge::uploadCreateDirectoryFinished
    );
}

void WebBridge::uploadCreateDirectoryFinished() {
    auto content = uploadReply->readAll();
    auto response = Response(content, RequestType::UPLOAD_NEW_FOLDER);

    auto error = uploadReply->error();
    if (error != QNetworkReply::NoError) {
        qDebug() << "error: " << error;

        delete currentUpload;
        currentUpload = nullptr;
        uploadReply->deleteLater();
        uploadReply = nullptr;
        emit responseError(error, response);

        triggerUpload();
    } else {
        uploadReply->deleteLater();
        uploadReply = nullptr;

        triggerUploadSendFile();
    }
}

void WebBridge::triggerUploadSendFile() {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart pathPart;
    pathPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"path\""));
    auto path = currentUpload->getRootServer() + "/" + currentUpload->getRelativeDir();
    pathPart.setBody(path.toUtf8());

    QHttpPart filePart;
    auto header = "form-data; name=\"file\"; filename=\"" + currentUpload->getFilename() + "\"";
    filePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QVariant(header)
    );
    auto pathLocal = currentUpload->getRootLocal() + "/" + currentUpload->getRelativePath();
    QFile *file = new QFile(pathLocal);
    bool fileOpened = file->open(QIODevice::ReadOnly);

    if (fileOpened) {
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
            &WebBridge::uploadSendFileFinished
        );
        connect(
            uploadReply,
            &QNetworkReply::uploadProgress,
            this,
            &WebBridge::uploadProgress
        );
    } else {
        delete currentUpload;
        currentUpload = nullptr;
        emit fileOpenError(pathLocal);
        triggerUpload();
    }
}

void WebBridge::uploadSendFileFinished() {
    auto content = uploadReply->readAll();
    auto response = Response(content, RequestType::UPLOAD_SEND_FILE);

    auto error = uploadReply->error();

    delete currentUpload;
    currentUpload = nullptr;
    uploadReply->deleteLater();
    uploadReply = nullptr;

    if (error != QNetworkReply::NoError) {
        qDebug() << "error: " << error;

        emit responseError(error, response);
    }

    triggerUpload();
}
