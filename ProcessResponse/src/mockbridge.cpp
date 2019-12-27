#include "include/mockbridge.h"

MockBridge::MockBridge() : response(Response(0, QByteArray(), Response::Type::LOGIN)) {

}

void MockBridge::requestLogin(QString user, QString password) {
    emit gotResponse(response);
}

void MockBridge::requestGroups() {
    emit gotResponse(response);
}

void MockBridge::setResponse(Response response) {
    this->response = response;
}
