#ifndef REQUEST_H
#define REQUEST_H

#include <QNetworkRequest>
#include "include/requesttype.h"

/*!
 * \brief Request przechowuje informacje o żądaniu HTTP.
 */
class Request {
public:
    /*!
     * \brief Metoda HTTP
     */
    enum Method {
        GET,    /*!< Metoda GET */
        POST,   /*!< Metoda POST */
        DELETE  /*!< Metoda DELETE */
    };

    Request(const Method &method, const QNetworkRequest &request, const RequestType &type);
    void setData(const QByteArray &data);
    Method getMethod();
    QNetworkRequest getRequest();
    RequestType getType();
    QByteArray getData();

private:
    Method method;
    QNetworkRequest request;
    RequestType type;
    QByteArray data;
};

#endif // REQUEST_H
