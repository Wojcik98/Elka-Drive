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
    View *view = nullptr;
    Model *model = nullptr;
    LoginDialog *loginDialog = nullptr;
    RegisterDialog *registerDialog = nullptr;
    GroupSettingsDialog *groupSettingsDialog = nullptr;

    void connectViewAndModel();
    QStringList getAllFiles(QDir path);
    void unauthorized();
    void forbidden();
    void unknownError();

signals:
    void closeApp();

public slots:
    void responseError(QNetworkReply::NetworkError error, Response response);
    void openRegister();
    void requestNewGroup();
    void requestDelete(const QModelIndex &index);
    void fileDoubleClicked(const QModelIndex &index);
    void fileClicked(const QModelIndex &index);
    void goBack();
    void refresh();
    void groupsReceived(QList<QStandardItem*> groups);
    void newGroupStatusCode();
    void pathReceived(QList<QStandardItem*> dir);
    void openGroupSettings(QModelIndex index);
    void resourceDeleted();
    void createNewFolder();
    void newFolderCreated();
    void uploadFile();
    void uploadFolder();
};

#endif // CONTROLLER_H
