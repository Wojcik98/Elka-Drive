#include "include/webbridge.h"
#include <QUrlQuery>
#include <QDebug>

WebBridge::WebBridge(QString mainUrl) : mainUrl(mainUrl) {

}

void WebBridge::requestLogin(QString user, QString password) {
    // TODO check if we are not already requesting something
    auto url = QUrl(mainUrl + "/login");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", user);
    postData.addQueryItem("password", password);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    requestType = Response::Type::LOGIN;

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyFinished
    );
    connect(
        reply,
        &QIODevice::readyRead,
        this,
        &WebBridge::networkReplyReady
    );
}

void WebBridge::networkReplyFinished() {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if (reply->error()) {
        // TODO try again, if still failing show pop-up
    }

    Response response(statusCode.toInt(), dataRead, requestType);
    emit gotResponse(response);

    reply->deleteLater();
}

void WebBridge::networkReplyReady() {
    dataRead.append(reply->readAll());
}
