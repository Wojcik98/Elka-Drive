#ifndef RESPONSE_H
#define RESPONSE_H

#include <QByteArray>
#include <QString>
#include "include/requesttype.h"

class Model;

/*!
 * \brief Response przechowuje informacje o odpowiedzi na żądanie HTTP.
 */
class Response {
public:
    Response(const QByteArray &body, const RequestType &type);
    QByteArray getBody() const;
    RequestType getType() const;

private:
    QByteArray body;
    RequestType type;
};

#endif // RESPONSE_H
