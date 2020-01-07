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

    void tryLogin();
    void showGroups();

private:
    QApplication *app;
    View *view;
    Model *model;
    LoginDialog *loginDialog;
    RegisterDialog *registerDialog;
    GroupSettingsDialog *groupSettingsDialog;

    QStringList getAllFiles(QDir path);

signals:
    void closeApp();

public slots:
    void unauthorized();
    void responseError(QNetworkReply::NetworkError error);
    void openRegister();
    void requestNewGroup();
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);
    void goBack();
    void refresh();
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode(int statusCode);
    void pathReceived(QList<QStandardItem*> dir, bool forbidden);
    void openGroupSettings(QModelIndex index);
    void resourceDeleted(bool success, bool notFound, bool forbidden);
    void createNewFolder();
    void newFolderCreated(bool success, bool forbidden);
    void uploadFile();
    void uploadFolder();
};

#endif // CONTROLLER_H
