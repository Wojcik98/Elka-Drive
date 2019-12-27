#ifndef APIBRIDGE_H
#define APIBRIDGE_H

#include <QJsonObject>
#include <QObject>
#include <QString>
#include "response.h"

class APIBridge : public QObject {
Q_OBJECT

public:
    APIBridge();
    virtual ~APIBridge();
    virtual void requestLogin(QString user, QString password) = 0;
    virtual void requestGroups() = 0;

signals:
    void gotResponse(Response response);    // TODO reference?
};

#endif // APIBRIDGE_H
