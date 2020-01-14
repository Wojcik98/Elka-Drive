#include "include/request.h"

/*!
 * \brief Konstruktor.
 * \param method Metoda HTTP
 * \param request Informacje o żądaniu
 * \param type Typ żądania
 */
Request::Request(const Method &method, const QNetworkRequest &request, const RequestType &type)
    : method(method), request(request), type(type) {

}

/*!
 * \param data Dane do wysłania
 */
void Request::setData(const QByteArray &data) {
    this->data = data;
}

/*!
 * \return Metoda HTTP
 */
Request::Method Request::getMethod() {
    return method;
}

/*!
 * \return Informacje o żądaniu
 */
QNetworkRequest Request::getRequest() {
    return request;
}

/*!
 * \return Typ żądania
 */
RequestType Request::getType() {
    return type;
}

/*!
 * \return Dane do wysłania
 */
QByteArray Request::getData() {
    return data;
}
