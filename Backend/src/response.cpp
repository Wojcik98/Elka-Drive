#include "include/response.h"

Response::Response(int statusCode, QByteArray body, Response::Type type, QString name) : statusCode(statusCode), body(body), type(type), name(name) {
}

int Response::getStatus() {
    return statusCode;
}

QByteArray Response::getBody() {
    return body;
}

Response::Type Response::getType() {
    return type;
}

QString Response::getName() {
    return name;
}
