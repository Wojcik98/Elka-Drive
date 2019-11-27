#include "mockbridge.h"

MockBridge::MockBridge() {

}

QJsonObject MockBridge::requestLogin(QString user, QString password) {
    return response;
}

void MockBridge::setResponse(QJsonObject response) {
    this->response = response;
}
