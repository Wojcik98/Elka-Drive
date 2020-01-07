#include "include/mockbridge.h"
#include "include/requesttype.h"

MockBridge::MockBridge() : response(Response(QByteArray(), RequestType::LOGIN)) {

}

void MockBridge::requestLogin(QString, QString) {
    emit gotResponse(response);
}

void MockBridge::requestRegister(QString, QString) {
    emit gotResponse(response);
}

void MockBridge::requestGroups() {
    emit gotResponse(response);
}

void MockBridge::requestNewGroup(QString) {
    emit gotResponse(response);
}

void MockBridge::requestPath(QString) {
    emit gotResponse(response);
}

void MockBridge::requestFileDownload(int, QString) {
    emit gotResponse(response);
}

void MockBridge::requestDirectoryDownload(int, QString) {
    emit gotResponse(response);
}

void MockBridge::requestGroupUsers(int) {
    emit gotResponse(response);
}

void MockBridge::requestGroupDelete(int) {
    emit gotResponse(response);
}

void MockBridge::requestAddUserToGroup(QString, int) {
    emit gotResponse(response);
}

void MockBridge::requestRemoveUserFromGroup(QString, int) {
    emit gotResponse(response);
}

void MockBridge::requestFileDelete(int) {
    emit gotResponse(response);
}

void MockBridge::requestDirectoryDelete(QString) {
    emit gotResponse(response);
}

void MockBridge::requestNewFolder(QString) {
    emit gotResponse(response);
}


void MockBridge::requestFileUpload(QString, QString, QString) {
    emit gotResponse(response);
}

void MockBridge::sendMsg(int, const QString&) {
    emit gotResponse(response);
}

void MockBridge::setResponse(Response response) {
    this->response = response;
}
