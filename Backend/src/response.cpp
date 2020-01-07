#include "include/response.h"

Response::Response(QByteArray body, RequestType type) : body(body), type(type) {
}

QByteArray Response::getBody() {
    return body;
}

RequestType Response::getType() {
    return type;
}
