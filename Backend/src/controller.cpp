#include <QtDebug>

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
        &Model::groupsReceived,
        this,
        &Controller::groupsReceived
    );
}

void Controller::setView(View *view) {
    this->view = view;
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

    loginDialog->exec();
}

void Controller::slotTryUserLogin(QString& user, QString& password) {
    model->requestLogin(user, password);
}

void Controller::loginDialogClosed() {
    loginDialog->deleteLater();

    if (!model->isLogged()) {
        emit closeApp();
    }

    qDebug("groups");
    model->requestGroups();
}

void Controller::groupsReceived(QList<QStandardItem*> groups) {
    view->setFileList(groups);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto filename = index.data().toString();
    qDebug() << "download: " << filename;
    model->requestDownload(path.join("/") + filename);
}

void Controller::requestDelete(const QModelIndex &index) {
    auto filename = index.data().toString();
    model->requestDelete(path.join("/") + filename);
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    auto filename = index.data().toString();
    auto type = index.data(Qt::UserRole + 1).toString();
    qDebug() << "clicked: " << filename;

    if (type == "folder" || type == "group") {
        path.append(filename);
        qDebug() << "get " << path.join("/");
        auto result = model->getPath(path.join("/"));
        view->setFileList(result);
    } else {
        requestDownload(index);
    }
}

void Controller::goBack() {
    qDebug() << "Go back!";
    if (path.length() > 1) {
        path.pop_back();
        qDebug() << "get " << path.join("/");
        auto result = model->getPath(path.join("/"));
        view->setFileList(result);
    } else if (path.length() == 1) {
        path.pop_back();
        qDebug() << "get groups";
        model->requestGroups();
    }
}
