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
    explicit Controller(QApplication *app, View *view, Model *model);
    void requestDownload(const QModelIndex &index);

    void tryLogin();
    void showGroups();

private:
    QApplication *app;
    View *view;
    Model *model;
    LoginDialog *loginDialog = nullptr;
    RegisterDialog *registerDialog = nullptr;
    GroupSettingsDialog *groupSettingsDialog = nullptr;

    void connectApp();
    void connectView();
    void connectModel();
    void connectViewAndModel();
    QStringList getAllFiles(const QDir &path);
    void unauthorized();
    void forbidden();
    void directoryNotFound();
    void unknownError();

signals:
    void closeApp();

public slots:
    void responseError(const QNetworkReply::NetworkError &error, const Response &response);
    void openRegister();
    void requestNewGroup();
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);
    void goBack();
    void refresh();
    void groupsReceived(const QList<QStandardItem*> &groups);
    void newGroupStatusCode();
    void pathReceived(const QList<QStandardItem*> &dirs);
    void openGroupSettings(const QModelIndex &index);
    void createNewFolder();
    void uploadFile();
    void uploadFolder();
    void fileOpenError(const QString &filename);
    void sendMsg(const QString &msg);
};

#endif // CONTROLLER_H
