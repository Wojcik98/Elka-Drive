#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model(APIBridge *bridge);

    bool isLogged();

    void requestLogin(QString user, QString password);
    void requestDelete(QString path);
    void requestDownload(QString path);
    void requestPath(QString path);
    void requestGroups();

    static const int TYPE_ROLE = Qt::UserRole + 1;
    static const int ID_ROLE = Qt::UserRole + 2;

private:
    void handleLoginResponse(Response response);
    void handleGroupsResponse(Response response);
    void handlePathResponse(Response response);

    APIBridge *bridge;
    bool logged = false;

signals:
    void loginStatus(bool success);
    void groupsReceived(QList<QStandardItem*> groups);
    void pathReceived(QList<QStandardItem*> groups);

public slots:
    void gotResponse(Response response);
};

#endif // MODEL_H
