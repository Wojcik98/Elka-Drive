#include "include/request.h"

Request::Request(Method method, QNetworkRequest request, Response::Type type) : method(method), request(request), type(type) {

}

void Request::setData(QByteArray data) {
    this->data = data;
}

Request::Method Request::getMethod() {
    return method;
}

QNetworkRequest Request::getRequest() {
    return request;
}

Response::Type Request::getType() {
    return type;
}

QByteArray Request::getData() {
    return data;
}
