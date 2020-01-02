#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QApplication>
#include <QObject>

#include "include/logindialog.h"
#include "include/registerdialog.h"
#include "include/groupsettingsdialog.h"
#include "include/model.h"
#include "include/view.h"

class View;

class Controller : public QObject {
    Q_OBJECT
public:
    explicit Controller(QApplication *app);
    void requestDownload(const QModelIndex &index);

    void setView(View *view);
    void setModel(Model *model);

    void checkLogin();
    void showGroups();

private:
    QApplication *app;
    View *view;
    Model *model;
    LoginDialog *loginDialog;
    RegisterDialog *registerDialog;
    GroupSettingsDialog *groupSettingsDialog;

    QStringList path;

signals:
    void closeApp();
    void loginSuccess(bool success);
    void registerSuccess(bool success);
    void groupUsersReceived(QList<User> users);

public slots:
    void openRegister();
    void requestNewGroup();
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);
    void goBack();
    void refresh();
    void slotTryUserLogin(QString &user, QString &password);
    void slotTryRegister(QString &user, QString &password);
    void loginDialogClosed();
    void registerDialogClosed();
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode(int statusCode);
    void pathReceived(QList<QStandardItem*> dir);
    void openGroupSettings(QModelIndex index);
    void requestGroupUsers(int groupId);
};

#endif // CONTROLLER_H
