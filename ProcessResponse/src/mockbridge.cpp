#include "include/mockbridge.h"

MockBridge::MockBridge() : response(Response(0, QByteArray(), Response::Type::LOGIN, "")) {

}

void MockBridge::requestLogin(QString user, QString password) {
    emit gotResponse(response);
}

void MockBridge::requestRegister(QString user, QString password) {
    emit gotResponse(response);
}

void MockBridge::requestGroups() {
    emit gotResponse(response);
}

void MockBridge::requestNewGroup(QString groupName) {
    emit gotResponse(response);
}

void MockBridge::requestPath(QString path) {
    emit gotResponse(response);
}

void MockBridge::requestFileDownload(int id, QString path) {
    emit gotResponse(response);
}

void MockBridge::requestDirectoryDownload(int id, QString path) {
    emit gotResponse(response);
}

void MockBridge::requestGroupUsers(int groupId) {
    emit gotResponse(response);
}

void MockBridge::requestGroupDelete(int groupId) {
    emit gotResponse(response);
}

void MockBridge::requestAddUserToGroup(QString username, int groupId) {
    emit gotResponse(response);
}

void MockBridge::requestRemoveUserFromGroup(QString username, int groupId) {
    emit gotResponse(response);
}

void MockBridge::requestFileDelete(int id) {
    emit gotResponse(response);
}

void MockBridge::requestDirectoryDelete(QString id) {
    emit gotResponse(response);
}

void MockBridge::requestNewFolder(QString path) {
    emit gotResponse(response);
}

void MockBridge::setResponse(Response response) {
    this->response = response;
}
