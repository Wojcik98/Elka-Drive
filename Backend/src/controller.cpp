#include <QtDebug>
#include <QPushButton>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTreeView>

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
        &Model::unauthorized,
        this,
        &Controller::unauthorized
    );
    connect(
        model,
        &Model::responseError,
        this,
        &Controller::responseError
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
        &Model::resourceDeleted,
        this,
        &Controller::resourceDeleted
    );
    connect(
        model,
        &Model::newFolderCreated,
        this,
        &Controller::newFolderCreated
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
    connect(
        view,
        &View::createNewFolder,
        this,
        &Controller::createNewFolder
    );
    connect(
        view,
        &View::uploadFile,
        this,
        &Controller::uploadFile
    );
    connect(
        view,
        &View::uploadFolder,
        this,
        &Controller::uploadFolder
    );
}

void Controller::responseError(QNetworkReply::NetworkError error) {
    QMessageBox msgBox;
    msgBox.setText("Error while connecting to the server!");
    msgBox.setInformativeText("Try again. If error persists contact developer.");
    msgBox.exec();
}

void Controller::unauthorized() {
    model->setLogged(false);

    QMessageBox msgBox;
    msgBox.setText("You have been logged out!");
    msgBox.exec();

    tryLogin();
}

void Controller::tryLogin() {
    if (model->isLogged()) {
        return;
    }
    model->clearPath();
    loginDialog = new LoginDialog();

    connect(
        loginDialog,
        &LoginDialog::tryLogin,
        model,
        &Model::requestLogin
    );
    connect(
        model,
        &Model::loginStatus,
        loginDialog,
        &LoginDialog::slotLoginResponse
    );
    connect(
        loginDialog,
        &LoginDialog::openRegister,
        this,
        &Controller::openRegister
    );

    loginDialog->exec();
    loginDialog->deleteLater();
    loginDialog = nullptr;

    if (!model->isLogged()) {
        emit closeApp();
    }

    model->requestGroups();
}

void Controller::openRegister() {
    registerDialog = new RegisterDialog();

    connect(
        registerDialog,
        &RegisterDialog::tryLogin,
        model,
        &Model::requestRegister
    );
    connect(
        model,
        &Model::registerStatus,
        registerDialog,
        &RegisterDialog::slotRegisterResponse
    );

    registerDialog->exec();
    registerDialog->deleteLater();
    registerDialog = nullptr;
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
        } else if (STATUS_SERVER_ERROR_LOW <     statusCode && statusCode <= STATUS_SERVER_ERROR_HIGH) {
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
        model,
        &Model::requestGroupUsers
    );
    connect(
        model,
        &Model::groupUsersReceived,
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

    model->requestGroupUsers(groupId);

    groupSettingsDialog->exec();
    groupSettingsDialog->deleteLater();
    groupSettingsDialog = nullptr;

    model->requestGroups();
}

void Controller::pathReceived(QList<QStandardItem*> path, bool forbidden) {
    if (forbidden) {
        QMessageBox msgBox;
        msgBox.setText("You were removed from this group!");
        msgBox.exec();
        model->requestGroups();
    } else {
        view->setFileList(path);
        view->setFilesButtonsVisible();
    }
}

void Controller::requestDelete(const QModelIndex &index) {
    model->requestDelete(index);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto type = index.data(Model::TYPE_ROLE).toInt();
    auto filename = index.data(Qt::DisplayRole).toString();

    if (type == Model::ItemType::FOLDER) {
        filename.append(".zip");
    }

    auto path = QFileDialog::getSaveFileName(view, "Save file", filename);
    // TODO check if empty
    model->requestDownload(index, path);
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
    model->requestSubpath(index);
}

void Controller::fileClicked(const QModelIndex&) {
    view->setSettingsButtonEnabled(true);
}

void Controller::goBack() {
    model->goBack();
}

void Controller::refresh() {
    model->refresh();
}

void Controller::resourceDeleted(bool success, bool notFound, bool forbidden) {
    if (notFound) {
        QMessageBox msgBox(view);
        msgBox.setText("File does not exist!");
        msgBox.exec();
        refresh();
    } else if (forbidden) {
        QMessageBox msgBox(view);
        msgBox.setText("You were removed from this group!");
        msgBox.exec();
        model->requestGroups();
    } else if (success) {
        refresh();
    } else {
        QMessageBox msgBox(view);
        msgBox.setText("Unknown error!");
        msgBox.exec();
        refresh();
    }
}

void Controller::createNewFolder() {
    bool ok;

    QString name = QInputDialog::getText(
        view,
        "New folder",
        "Name:",
        QLineEdit::Normal,
        "",
        &ok
    );

    if (ok) {
        model->requestNewFolder(name);
    }
}

void Controller::newFolderCreated(bool success, bool forbidden) {
    if (forbidden) {
        // TODO extract and clear path
        QMessageBox msgBox(view);
        msgBox.setText("You were removed from this group!");
        msgBox.exec();
        model->requestGroups();
    } else if (success) {
        refresh();
    } else {
        // TODO extract
        QMessageBox msgBox(view);
        msgBox.setText("Unknown error!");
        msgBox.exec();
        refresh();
    }
}

void Controller::uploadFile() {
    QStringList files = QFileDialog::getOpenFileNames(
        view, "Select one or more files to upload", QDir::homePath()
    );

    for (const auto &file : files) {
        QFileInfo info(file);
        model->requestFileUpload(info.absolutePath(), info.fileName());
    }
}

void Controller::uploadFolder() {
    QString chosenDir = QFileDialog::getExistingDirectory(
        view, "Open Directory", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    if (chosenDir.isEmpty()) {
        return;
    }

    QDir workingDir(chosenDir);
    auto files = getAllFiles(workingDir);

    workingDir.cdUp();
    auto absolute = workingDir.absolutePath();

    for (const auto &file : files) {
        auto relative = workingDir.relativeFilePath(file);
        model->requestFileUpload(absolute, relative);
    }
}

QStringList Controller::getAllFiles(QDir path) {
    QStringList files;
    auto info = path.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);

    for (const auto &item : info) {
        if (item.isDir() && !item.isSymLink()) {
            auto subfiles = getAllFiles(item.filePath());
            files.append(subfiles);
        } else if (!item.isDir()) {
            files.append(item.filePath());
        }
    }

    return files;
}
