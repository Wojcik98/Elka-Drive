#include "include/webbridge.h"
#include <QUrlQuery>
#include <QDebug>

WebBridge::WebBridge(QString mainUrl) : mainUrl(mainUrl) {
    reply = nullptr;
}

void WebBridge::requestLogin(QString user, QString password) {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
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

void WebBridge::requestGroups() {
    if (reply != nullptr) {
        qDebug() << "Another request in progress!";
        return;
    }
    dataRead.clear();
    auto url = QUrl(mainUrl + "/groups/my");
    auto request = QNetworkRequest(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    requestType = Response::Type::GROUPS;
    reply = manager.get(request);

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

    qDebug() << statusCode;
    if (reply->error()) {
        qDebug() << "error!";
        // TODO try again, if still failing show pop-up
    }

    Response response(statusCode.toInt(), dataRead, requestType);

    reply->deleteLater();
    reply = nullptr;
    emit gotResponse(response);
}

void WebBridge::networkReplyReady() {
    dataRead.append(reply->readAll());
}
