#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QObject>

#include "include/logindialog.h"
#include "include/model.h"
#include "include/view.h"

class View;

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QApplication *app);

    void setView(View *view);
    void setModel(Model *model);

    void checkLogin();
    void showGroups();

private:
    QApplication *app;
    View *view;
    Model *model;
    LoginDialog* loginDialog;

    QStringList path;

signals:
    void closeApp();
    void loginSuccess(bool success);

public slots:
    void requestDownload(const QModelIndex &index);
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void goBack();
    void slotTryUserLogin(QString& user, QString& password);
    void loginDialogClosed();
    void groupsReceived(QList<QStandardItem*> groups);
    void pathReceived(QList<QStandardItem*> dir);
};

#endif // CONTROLLER_H
