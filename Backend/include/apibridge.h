#ifndef APIBRIDGE_H
#define APIBRIDGE_H

#include <QJsonObject>
#include <QObject>
#include <QString>

class APIBridge : public QObject {
Q_OBJECT

public:
    APIBridge();
    virtual ~APIBridge();
    virtual void requestLogin(QString user, QString password) = 0;

signals:
    void gotReply(QJsonObject reply);
};

#endif // APIBRIDGE_H
