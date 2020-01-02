#include <QtDebug>
#include <QPushButton>
#include <QInputDialog>
#include <QMessageBox>

#include "include/controller.h"

Controller::Controller(QApplication *app) : app(app) {
    connect(
        this,
        &Controller::closeApp,
        app,
        &QApplication::quit,
        Qt::QueuedConnection
    );
}

void Controller::setModel(Model *model) {
    this->model = model;

    connect(
        model,
        &Model::loginStatus,
        this,
        &Controller::loginSuccess
    );
    connect(
        model,
        &Model::registerStatus,
        this,
        &Controller::registerSuccess
    );
    connect(
        model,
        &Model::groupsReceived,
        this,
        &Controller::groupsReceived
    );
    connect(
        model,
        &Model::newGroupStatusCode,
        this,
        &Controller::newGroupStatusCode
    );
    connect(
        model,
        &Model::pathReceived,
        this,
        &Controller::pathReceived
    );
    connect(
        model,
        &Model::groupUsersReceived,
        this,
        &Controller::groupUsersReceived
    );
}

void Controller::setView(View *view) {
    this->view = view;

    connect(
        view,
        &View::createNewGroup,
        this,
        &Controller::requestNewGroup
    );
    connect(
        view,
        &View::openGroupSettings,
        this,
        &Controller::openGroupSettings
    );
}

void Controller::checkLogin() {
    if (model->isLogged()) {
        return;
    }
    loginDialog = new LoginDialog();

    connect(
        loginDialog,
        &LoginDialog::tryLogin,
        this,
        &Controller::slotTryUserLogin
    );
    connect(
        this,
        &Controller::loginSuccess,
        loginDialog,
        &LoginDialog::slotLoginResponse
    );
    connect(
        loginDialog,
        &LoginDialog::rejected,
        this,
        &Controller::loginDialogClosed
    );
    connect(
        loginDialog,
        &LoginDialog::openRegister,
        this,
        &Controller::openRegister
    );

    loginDialog->exec();
}

void Controller::slotTryUserLogin(QString& user, QString& password) {
    model->requestLogin(user, password);
}

void Controller::slotTryRegister(QString& user, QString& password) {
    model->requestRegister(user, password);
}

void Controller::loginDialogClosed() {
    loginDialog->deleteLater();

    if (!model->isLogged()) {
        emit closeApp();
    }

    model->requestGroups();
}

void Controller::registerDialogClosed() {
    registerDialog->deleteLater();
}

void Controller::openRegister() {
    registerDialog = new RegisterDialog();
    // TODO delete

    connect(
        registerDialog,
        &RegisterDialog::tryLogin,
        this,
        &Controller::slotTryRegister
    );
    connect(
        this,
        &Controller::registerSuccess,
        registerDialog,
        &RegisterDialog::slotRegisterResponse
    );
    connect(
        registerDialog,
        &RegisterDialog::rejected,
        this,
        &Controller::registerDialogClosed
    );

    registerDialog->exec();
}

void Controller::groupsReceived(QList<QStandardItem*> groups) {
    view->setFileList(groups);
    view->setGroupsButtonsVisible();
    view->setSettingsButtonEnabled(false);
}

void Controller::newGroupStatusCode(int statusCode) {
    const int STATUS_OK = 200;
    const int STATUS_NAME_EXIST = 409;
    const int STATUS_SERVER_ERROR_LOW = 500;
    const int STATUS_SERVER_ERROR_HIGH = 599;

    if (statusCode == STATUS_OK) {
        model->requestGroups();
    } else {
        QMessageBox msgBox;
        QString text;
        if (statusCode == STATUS_NAME_EXIST) {
            text = "Group with this name already exists.";
        } else if (STATUS_SERVER_ERROR_LOW < statusCode && statusCode <= STATUS_SERVER_ERROR_HIGH) {
            text = "Server error, try again. If error persists contact developer.";
        } else {
            text = "Unknown error, try again. If error persists contact developer.";
        }
        msgBox.setText(text);
        msgBox.exec();
    }
}

void Controller::openGroupSettings(QModelIndex index) {
    QString groupName = index.data(Qt::DisplayRole).toString();
    int groupId = index.data(Model::ID_ROLE).toInt();

    groupSettingsDialog = new GroupSettingsDialog(groupName, groupId, view);

    connect(
        groupSettingsDialog,
        &GroupSettingsDialog::requestGroupUsers,
        this,
        &Controller::requestGroupUsers
    );
    connect(
        this,
        &Controller::groupUsersReceived,
        groupSettingsDialog,
        &GroupSettingsDialog::groupUsersReceived
    );
    connect(
        groupSettingsDialog,
        &GroupSettingsDialog::requestGroupDelete,
        model,
        &Model::requestGroupDelete
    );
    connect(
        model,
        &Model::groupDeletedReceived,
        groupSettingsDialog,
        &GroupSettingsDialog::groupDeletedReceived
    );
    connect(
        groupSettingsDialog,
        &GroupSettingsDialog::requestAddUserToGroup,
        model,
        &Model::requestAddUserToGroup
    );
    connect(
        groupSettingsDialog,
        &GroupSettingsDialog::requestRemoveUserFromGroup,
        model,
        &Model::requestRemoveUserFromGroup
    );
    connect(
        model,
        &Model::groupAddUserReceived,
        groupSettingsDialog,
        &GroupSettingsDialog::groupAddUserReceived
    );
    connect(
        model,
        &Model::groupRemoveUserReceived,
        groupSettingsDialog,
        &GroupSettingsDialog::groupRemoveUserReceived
    );

    requestGroupUsers(groupId);
    groupSettingsDialog->exec();
    model->requestGroups();
}

void Controller::requestGroupUsers(int groupId) {
    model->requestGroupUsers(groupId);
}

void Controller::pathReceived(QList<QStandardItem*> path) {
    view->setFileList(path);
    view->setFilesButtonsVisible();
}

void Controller::requestDelete(const QModelIndex &index) {
    auto filename = index.data().toString();
    model->requestDelete(path.join("/") + filename);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto id = index.data(Model::ID_ROLE).toInt();
    qDebug() << "Download " << id;
    model->requestDownload(QString::number((id)));
}

void Controller::requestNewGroup() {
    bool ok;

    QString groupName = QInputDialog::getText(
        view,
        "Create new group",
        "Group name:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (ok) {
        model->requestNewGroup(groupName);
    }
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    auto filename = index.data().toString();
    auto type = index.data(Model::TYPE_ROLE).toString();
    qDebug() << "clicked: " << filename;

    if (type == "group") {
        auto id = index.data(Model::ID_ROLE).toString();
        path.append(id);
        qDebug() << "get " << path.join("/");
        model->requestPath(path.join("/"));
    } else if (type == "folder") {
        path.append(filename);
        qDebug() << "get " << path.join("/");
        model->requestPath(path.join("/"));
    } else {
        requestDownload(index);
    }
}

void Controller::fileClicked(const QModelIndex&) {
    view->setSettingsButtonEnabled(true);
}

void Controller::goBack() {
    if (path.length() > 1) {
        path.pop_back();
        model->requestPath(path.join("/"));
    } else if (path.length() == 1) {
        path.pop_back();
        model->requestGroups();
    }
}

void Controller::refresh() {
    if (path.length() > 0) {
        model->requestPath(path.join("/"));
    } else {
        model->requestGroups();
    }
}
