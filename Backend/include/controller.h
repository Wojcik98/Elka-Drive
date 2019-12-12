#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

#include "include/logindialog.h"
#include "include/model.h"
#include "include/view.h"

class View;

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller();

    void setView(View *view);
    void setModel(Model *model);

    void checkLogin();
    void showGroups();

private:
    View *view;
    Model *model;

    QStringList path;

signals:
    void loginSuccess(bool success);

public slots:
    void requestDownload(const QModelIndex &index);
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void goBack();
    void slotTryUserLogin(QString& user, QString& password);
};

#endif // CONTROLLER_H
