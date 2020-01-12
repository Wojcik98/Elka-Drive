#include <QtDebug>
#include <QDir>

#include "include/controller.h"

Controller::Controller(QApplication *app, View *view, Model *model) : app(app), view(view), model(model) {
    connectApp();
    connectView();
    connectModel();
    connectViewAndModel();
}

void Controller::connectApp() {
    connect(
        this,
        &Controller::closeApp,
        app,
        &QApplication::quit,
        Qt::QueuedConnection
    );
}

void Controller::connectView() {
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
    connect(
        view,
        &View::sendMsg,
        this,
        &Controller::sendMsg
    );
}

void Controller::connectModel() {
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
    connect(
        model,
        &Model::fileOpenError,
        this,
        &Controller::fileOpenError
    );
}

void Controller::connectViewAndModel() {
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

void Controller::responseError(const QNetworkReply::NetworkError &error, const Response &response) {
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
        case QNetworkReply::ContentNotFoundError:
            directoryNotFound();
            break;
        default:
            unknownError();
            break;
    }

    if (loginDialog) loginDialog->enableButton(true);
    if (registerDialog) registerDialog->enableButton(true);
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

void Controller::directoryNotFound() {
    view->showDirectoryNotFound();
    model->goBack();
}

void Controller::unknownError() {
    view->showUnknownErrorMsg();
}

void Controller::tryLogin() {
    if (model->isLogged()) {
        return;
    }
    model->clearPath();
    model->clearMsgs();

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
        &LoginDialog::loginCorrect
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
    } else {
        model->requestGroups();
    }
}

void Controller::openRegister() {
    registerDialog = new RegisterDialog();

    connect(
        registerDialog,
        &RegisterDialog::tryRegister,
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

void Controller::groupsReceived(const QList<QStandardItem*> &groups) {
    view->setFileList(groups);
    view->setGroupsView();
    view->setSettingsButtonEnabled(false);
    view->setChatModel(nullptr);
}

void Controller::newGroupStatusCode() {
    model->requestGroups();
}

void Controller::openGroupSettings(const QModelIndex &index) {
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

void Controller::pathReceived(const QList<QStandardItem*> &path) {
    view->setFileList(path);
    view->setFilesView();
    view->setChatModel(model->getCurrentGroupMessages());
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

QStringList Controller::getAllFiles(const QDir &path) {
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

void Controller::fileOpenError(const QString &filename) {
    view->showFileOpenError(filename);
}

void Controller::sendMsg(const QString &msg) {
    model->sendMsg(msg);
    view->clearMsg();
}
