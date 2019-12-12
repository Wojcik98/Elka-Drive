#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model(APIBridge *bridge);

    bool requestLogin(QString user, QString password);
    bool isLogged();

    QList<QStandardItem*> getGroups();
    QList<QStandardItem*> getPath(QString path);

    void requestDelete(QString path);
    void requestDownload(QString path);
    void requestPath(QString path);

private:
    APIBridge *bridge;
    bool logged = false;

signals:
    void loginResponse(bool success);

public slots:
    void slotLoginResponse(bool success);
    void newItems(QList<QStandardItem*>&);
};

#endif // MODEL_H
