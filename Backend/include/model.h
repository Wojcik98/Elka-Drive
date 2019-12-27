#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model(APIBridge *bridge);

    bool isLogged();

    QList<QStandardItem*> getPath(QString path);

    void requestLogin(QString user, QString password);
    void requestDelete(QString path);
    void requestDownload(QString path);
    void requestPath(QString path);
    void requestGroups();

private:
    void handleLoginResponse(Response response);
    void handleGroupsResponse(Response response);

    APIBridge *bridge;
    bool logged = false;

signals:
    void loginStatus(bool success);
    void groupsReceived(QList<QStandardItem*> groups);

public slots:
    void gotResponse(Response response);
};

#endif // MODEL_H
