#include "include/mockbridge.h"

MockBridge::MockBridge() {

}

void MockBridge::requestLogin(QString user, QString password) {
//    return response;
    // TODO impl
}

void MockBridge::setResponse(QJsonObject response) {
    this->response = response;
}
