#include "include/mockbridge.h"
#include "include/requesttype.h"

MockBridge::MockBridge() : response(Response(QByteArray(), RequestType::LOGIN)) {

}

void MockBridge::requestLogin(QString, QString) {
    takeAction();
}

void MockBridge::requestRegister(QString, QString) {
    takeAction();
}

void MockBridge::requestGroups() {
    takeAction();
}

void MockBridge::requestNewGroup(QString) {
    takeAction();
}

void MockBridge::requestPath(QString) {
    takeAction();
}

void MockBridge::requestFileDownload(int, QString) {
    takeAction();
}

void MockBridge::requestDirectoryDownload(int, QString) {
    takeAction();
}

void MockBridge::requestGroupUsers(int) {
    takeAction();
}

void MockBridge::requestGroupDelete(int) {
    takeAction();
}

void MockBridge::requestAddUserToGroup(QString, int) {
    takeAction();
}

void MockBridge::requestRemoveUserFromGroup(QString, int) {
    takeAction();
}

void MockBridge::requestFileDelete(int) {
    takeAction();
}

void MockBridge::requestDirectoryDelete(QString) {
    takeAction();
}

void MockBridge::requestNewFolder(QString) {
    takeAction();
}


void MockBridge::requestFileUpload(QString, QString, QString) {
    takeAction();
}

void MockBridge::sendMsg(int, const QString&) {
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

void MockBridge::emitGotResponse(Response response) {
    action = GOT_RESPONSE;
    this->response = response;
}

void MockBridge::emitResponseError(QNetworkReply::NetworkError error, Response response) {
    action = RESPONSE_ERROR;
    this->error = error;
    this->response = response;
}

void MockBridge::emitDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    action = DOWNLOAD_PROGRESS;
    this->current = bytesReceived;
    this->total = bytesTotal;
}

void MockBridge::emitUploadProgress(qint64 bytesSent, qint64 bytesTotal) {
    action = UPLOAD_PROGRESS;
    this->current = bytesSent;
    this->total = bytesTotal;
}

void MockBridge::emitFileOpenError(const QString &filename) {
    action = FILE_OPEN_ERROR;
    this->filename = filename;
}
