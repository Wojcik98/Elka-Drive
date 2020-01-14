#include <QtDebug>
#include <QDir>

#include "include/controller.h"

/*!
 * \brief Konstruktor.
 * \param app Aplikacja Qt
 * \param view Widok wzorca MVC
 * \param model Model wzorca MVC
 */
Controller::Controller(QApplication *app, View *view, Model *model)
    : app(app), view(view), model(model) {
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
    connect(
        view,
        &View::requestDownload,
        this,
        &Controller::requestDownload
    );
    connect(
        view,
        &View::requestDelete,
        this,
        &Controller::requestDelete
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
        &Model::newGroupCreated,
        this,
        &Controller::newGroupCreated
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

/*!
 * \brief Obsługuje błąd zwrócony przy żądaniu HTTP
 * \param error Błąd
 * \param response Odpowiedź z serwera
 */
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
            } else if (type == RequestType::GROUP_USERS) {
                groupSettingsDialog->close();
                forbidden();
            } else {
                forbidden();
            }
            break;
        case QNetworkReply::ContentConflictError:
            if (type == RequestType::REGISTER) {
                if (registerDialog) registerDialog->userExists();
            } else if (type == RequestType::NEW_GROUP) {
                groupExist();
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

void Controller::groupExist() {
    view->showGroupExist();
    model->refresh();
}

void Controller::unknownError() {
    view->showUnknownErrorMsg();
}

/*!
 * \brief Otwiera okno logowania. Jeśli zostanie ono zamknięte
 * bez udanego logowania, zamyka aplikację.
 */
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

/*!
 * \brief Otwiera okno rejestracji.
 */
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

/*!
 * \brief Ustawia widok do wyświetlenia przekazanych grup
 * \param groups Grupy do wyświetlenia
 */
void Controller::groupsReceived(const QList<QStandardItem*> &groups) {
    auto path = model->getPath();
    view->setFileList(groups, path);
    view->setGroupsView();
    view->anyItemSelected(false);
    view->setChatModel(nullptr);
}

/*!
 * \brief Żąda zaktualizowanych informacji o grupach po stworzeniu nowej.
 */
void Controller::newGroupCreated() {
    model->requestGroups();
}

/*!
 * \brief Otwiera okno ustawień grupy.
 * \param index Informacje o grupie
 */
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

/*!
 * \brief Ustawia widok do wyświetlenia przekazanej zawartości ścieżki.
 * \param dirs Zawartość ścieżki
 */
void Controller::pathReceived(const QList<QStandardItem*> &dirs) {
    auto path = model->getPath();
    view->setFileList(dirs, path);
    view->setFilesView();
    view->anyItemSelected(false);
    view->setChatModel(model->getCurrentGroupMessages());
}

/*!
 * \brief Przekazuje modelowi żądanie usunięcie pliku lub folderu.
 * \param index Informacje o obiekcie do usunięcia
 */
void Controller::requestDelete(const QModelIndex &index) {
    model->requestDelete(index);
}

/*!
 * \brief Żądanie pobrania pliku lub folderu.
 * \param index Informacje o obiekcie do pobrania
 *
 * Najpierw pobiera informacje z widoku o nazwie, pod jaką zapisać
 * pobierany plik a następnie przekazuje żądanie do modelu.
 */
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

/*!
 * \brief Żądanie stworzenia nowej grupy.
 *
 * Najpierw pobiera informacje z widoku o nazwie nowej grupy
 * a następnie przekazuje żądanie do modelu.
 */
void Controller::requestNewGroup() {
    bool ok;
    QString groupName = view->getNewGroupName(&ok);

    if (ok) {
        model->requestNewGroup(groupName);
    }
}

/*!
 * \brief Obsługuje podwójne kliknięcie obiektu.
 * \param index Kliknięty obiekt
 *
 * Wysyła zapytanie o zawartość podścieżki jeśli obiekt jest grupą lub folderem,
 * wysyła żądanie pobrania jeśli jest plikiem.
 */
void Controller::fileDoubleClicked(const QModelIndex &index) {
    auto type = index.data(Model::TYPE_ROLE).toInt();

    if (type == Model::ItemType::GROUP || type == Model::ItemType::FOLDER) {
        model->requestSubpath(index);
    } else if (type == Model::ItemType::FILE) {
        requestDownload(index);
    }
}

/*!
 * \brief Przekazuje widokowi informację, że jakiś obiekt został zaznaczony.
 */
void Controller::fileSelected(const QModelIndex&) {
    view->anyItemSelected(true);
}

/*!
 * \brief Przekazuje modelowi żądanie przejścia do wyższego folderu.
 */
void Controller::goBack() {
    model->goBack();
}

/*!
 * \brief Przekazuje modelowi żądanie odświeżenia.
 */
void Controller::refresh() {
    model->refresh();
}

/*!
 * \brief Żądanie stworzenia nowego folderu.
 *
 * Najpierw pobiera z widoku nazwę nowego folderu,
 * następnie przekazuje żądanie modelowi.
 */
void Controller::createNewFolder() {
    bool ok;
    QString name = view->getNewFolderName(&ok);

    if (ok) {
        model->requestNewFolder(name);
    }
}

/*!
 * \brief Żądanie wysłania pliku na serwer.
 *
 * Najpierw pobiera z widoku nazwę pliku do wysłania,
 * następnie przekazuje żądanie modelowi.
 */
void Controller::uploadFile() {
    QStringList files = view->getUploadFileNames();

    for (const auto &file : files) {
        QFileInfo info(file);
        model->requestFileUpload(info.absolutePath(), info.fileName());
    }
}

/*!
 * \brief Żądanie wysłania folderu na serwer.
 *
 * Najpierw pobiera z widoku nazwę folderu do wysłania,
 * następnie przekazuje żądanie modelowi.
 */
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

/*!
 * \brief Informuje widok o błędzie w otwieraniu pliku.
 * \param filename Nazwa pliku
 */
void Controller::fileOpenError(const QString &filename) {
    view->showFileOpenError(filename);
}

/*!
 * \brief Przekazuje modelowi żądanie wysłania wiadomości.
 * \param msg Tekst wiadomości do wysłania.
 */
void Controller::sendMsg(const QString &msg) {
    model->sendMsg(msg);
    view->clearMsg();
}
