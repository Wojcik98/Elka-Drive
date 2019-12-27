#include "include/response.h"

Response::Response(int statusCode, QByteArray body, Response::Type type) : statusCode(statusCode), body(body), type(type) {
}

int Response::getStatus() {
    return statusCode;
}

QByteArray Response::getBody() {
    return body;
}

Response::Type Response::getType(){
    return type;
}
