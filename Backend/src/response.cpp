#include "include/response.h"

Response::Response(const QByteArray &body, const RequestType &type)
    : body(body), type(type) {
}

QByteArray Response::getBody() const {
    return body;
}

RequestType Response::getType() const {
    return type;
}
