#include <QtDebug>
#include <QDir>

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
        &Controller::refresh
    );
    connect(
        model,
        &Model::newFolderCreated,
        this,
        &Controller::refresh
    );
    connect(
        model,
        &Model::uploadComplete,
        this,
        &Controller::refresh
    );

    connectViewAndModel();
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

    connectViewAndModel();
}

void Controller::connectViewAndModel() {
    if (view == nullptr || model == nullptr) {
        return;
    }

    connect(
        model,
        &Model::downloadProgress,
        view,
        &View::setDownloadProgress
    );
    connect(
        model,
        &Model::uploadProgress,
        view,
        &View::setUploadProgress
    );
}

void Controller::responseError(QNetworkReply::NetworkError error, Response response) {
    auto type = response.getType();
    switch (error) {
        case QNetworkReply::AuthenticationRequiredError:
            if (type == RequestType::LOGIN) {
                if (loginDialog) loginDialog->loginFailed();
            } else {
                unauthorized();
            }
            break;
        case QNetworkReply::ContentAccessDenied:
            if (type == RequestType::DELETE || type == RequestType::DOWNLOAD) {
                refresh();
            } else {
                forbidden();
            }
            break;
        case QNetworkReply::ContentConflictError:
            if (type == RequestType::REGISTER) {
                if (registerDialog) registerDialog->userExists();
            } else {
                unknownError();
            }
            break;
        default:
            unknownError();
            break;
    }
}

void Controller::unauthorized() {
    model->setLogged(false);
    view->showLogoutMsg();

    tryLogin();
}

void Controller::forbidden() {
    view->showForbiddenMsg();
    model->requestGroups();
}

void Controller::unknownError() {
    view->showUnknownErrorMsg();
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
        &Model::userLogged,
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
        &Model::userRegistered,
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

void Controller::newGroupStatusCode() {
    model->requestGroups();
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

void Controller::pathReceived(QList<QStandardItem*> path) {
    view->setFileList(path);
    view->setFilesButtonsVisible();
}

void Controller::requestDelete(const QModelIndex &index) {
    model->requestDelete(index);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto type = index.data(Model::TYPE_ROLE).toInt();
    auto suggested = index.data(Qt::DisplayRole).toString();

    if (type == Model::ItemType::FOLDER) {
        suggested.append(".zip");
    }

    auto path = view->getSaveFilename(suggested);
    if (!path.isEmpty()) {
        model->requestDownload(index, path);
    }
}

void Controller::requestNewGroup() {
    bool ok;
    QString groupName = view->getNewGroupName(&ok);

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

void Controller::createNewFolder() {
    bool ok;
    QString name = view->getNewFolderName(&ok);

    if (ok) {
        model->requestNewFolder(name);
    }
}

void Controller::uploadFile() {
    QStringList files = view->getUploadFileNames();

    for (const auto &file : files) {
        QFileInfo info(file);
        model->requestFileUpload(info.absolutePath(), info.fileName());
    }
}

void Controller::uploadFolder() {
    QString chosenDir = view->getUploadFolderName();
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
