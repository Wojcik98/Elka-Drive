#ifndef MODEL_H
#define MODEL_H

#include <QStandardItemModel>
#include "include/apibridge.h"

class Model : public QObject {
    Q_OBJECT

public:
    Model(APIBridge *bridge);

    bool login(QString user, QString password);

    QList<QStandardItem*> getGroups();
    QList<QStandardItem*> getPath(QString path);

    void requestDelete(QString path);
    void requestDownload(QString path);

private:
    APIBridge *bridge;
};

#endif // MODEL_H
