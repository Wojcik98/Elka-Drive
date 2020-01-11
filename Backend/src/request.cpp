#include "include/request.h"

Request::Request(const Method &method, const QNetworkRequest &request, const RequestType &type)
    : method(method), request(request), type(type) {

}

void Request::setData(const QByteArray &data) {
    this->data = data;
}

Request::Method Request::getMethod() {
    return method;
}

QNetworkRequest Request::getRequest() {
    return request;
}

RequestType Request::getType() {
    return type;
}

QByteArray Request::getData() {
    return data;
}
