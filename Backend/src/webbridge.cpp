#include "include/webbridge.h"
#include <QUrlQuery>
#include <QDebug>

WebBridge::WebBridge(QString mainUrl) : mainUrl(mainUrl) {

}

void WebBridge::requestLogin(QString user, QString password) {
    qDebug() << "requestin";
    auto url = QUrl(mainUrl + "/login");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("username", user);
    postData.addQueryItem("password", password);
    reply = manager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    connect(
        reply,
        &QNetworkReply::finished,
        this,
        &WebBridge::networkReplyProgress
    );
}

void WebBridge::networkReplyProgress() {
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    qDebug() << reply->readAll() << statusCode.toInt();
}
