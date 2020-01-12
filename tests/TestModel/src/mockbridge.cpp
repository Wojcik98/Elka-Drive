#include "include/mockbridge.h"
#include "include/requesttype.h"

MockBridge::MockBridge() : response(Response(QByteArray(), RequestType::LOGIN)) {

}

void MockBridge::requestLogin(const QString&, const QString&) {
    takeAction();
}

void MockBridge::requestRegister(const QString&, const QString&) {
    takeAction();
}

void MockBridge::requestGroups() {
    takeAction();
}

void MockBridge::requestNewGroup(const QString&) {
    takeAction();
}

void MockBridge::requestPath(const QString&) {
    takeAction();
}

void MockBridge::requestFileDownload(const int, const QString&) {
    takeAction();
}

void MockBridge::requestDirectoryDownload(const int, const QString&) {
    takeAction();
}

void MockBridge::requestGroupUsers(const int) {
    takeAction();
}

void MockBridge::requestGroupDelete(const int) {
    takeAction();
}

void MockBridge::requestAddUserToGroup(const QString&, const int) {
    takeAction();
}

void MockBridge::requestRemoveUserFromGroup(const QString&, const int) {
    takeAction();
}

void MockBridge::requestFileDelete(const int) {
    takeAction();
}

void MockBridge::requestDirectoryDelete(const QString&) {
    takeAction();
}

void MockBridge::requestNewFolder(const QString&) {
    takeAction();
}


void MockBridge::requestFileUpload(const QString&, const QString&, const QString&) {
    takeAction();
}

void MockBridge::sendMsg(const int, const QString&) {
    takeAction();
}

void MockBridge::takeAction() {
    switch (action) {
    case GOT_RESPONSE:
        emit gotResponse(response);
        break;
    case RESPONSE_ERROR:
        emit responseError(error, response);
        break;
    case DOWNLOAD_PROGRESS:
        emit downloadProgress(current, total);
        break;
    case UPLOAD_PROGRESS:
        emit uploadProgress(current, total);
        break;
    case FILE_OPEN_ERROR:
        emit fileOpenError(filename);
        break;
    }
}

void MockBridge::emitGotResponse(const Response &response) {
    action = GOT_RESPONSE;
    this->response = response;
}

void MockBridge::emitResponseError(const QNetworkReply::NetworkError &error, const Response &response) {
    action = RESPONSE_ERROR;
    this->error = error;
    this->response = response;
}

void MockBridge::emitDownloadProgress(const qint64 bytesReceived, const qint64 bytesTotal) {
    action = DOWNLOAD_PROGRESS;
    this->current = bytesReceived;
    this->total = bytesTotal;
}

void MockBridge::emitUploadProgress(const qint64 bytesSent, const qint64 bytesTotal) {
    action = UPLOAD_PROGRESS;
    this->current = bytesSent;
    this->total = bytesTotal;
}

void MockBridge::emitFileOpenError(const QString &filename) {
    action = FILE_OPEN_ERROR;
    this->filename = filename;
}
