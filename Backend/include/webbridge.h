#ifndef WEBBRIDGE_H
#define WEBBRIDGE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include "include/apibridge.h"

class WebBridge : public APIBridge {
public:
    WebBridge(QString mainUrl);
    virtual void requestLogin(QString user, QString password);

private:
    QString mainUrl;
    QNetworkAccessManager manager;
    QNetworkReply *reply;
    Response::Type requestType;
    QByteArray dataRead;

public slots:
    void networkReplyReady();
    void networkReplyFinished();
};

#endif // WEBBRIDGE_H
