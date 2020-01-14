#include "include/response.h"

/*!
 * \brief Konstruktor
 * \param body Odpowiedź
 * \param type Typ żądanie
 */
Response::Response(const QByteArray &body, const RequestType &type)
    : body(body), type(type) {
}

/*!
 * \return Odpowiedź
 */
QByteArray Response::getBody() const {
    return body;
}

/*!
 * \return Typ żądania
 */
RequestType Response::getType() const {
    return type;
}
