#include "include/view.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

View::View(QWidget *parent) : QMainWindow(parent), groupsWidget(this), filesWidget(this) {
    QHBoxLayout *groupsButtons = new QHBoxLayout();
    newButton = new QPushButton(QIcon(":/icons/add_circle.svg"), "New", this);
    settingsButton = new QPushButton(QIcon(":/icons/settings.svg"), "Settings", this);
    groupsButtons->addWidget(settingsButton);
    groupsButtons->addWidget(newButton);

    QHBoxLayout *filesButtons = new QHBoxLayout();
    dirButton = new QPushButton(QIcon(":/icons/create_new_folder.svg"), "New folder", this);
    uploadFileButton = new QPushButton(QIcon(":/icons/cloud_upload.svg"), "Upload file", this);
    uploadFolderButton = new QPushButton(QIcon(":/icons/cloud_upload.svg"), "Upload folder", this);
    filesButtons->addWidget(dirButton);
    filesButtons->addWidget(uploadFolderButton);
    filesButtons->addWidget(uploadFileButton);

    groupsWidget.setLayout(groupsButtons);
    groupsWidget.show();
    filesWidget.setLayout(filesButtons);
    filesWidget.hide();

    uploadProgress = new QProgressBar(this);
    downloadProgress = new QProgressBar(this);

    connect(
        newButton,
        &QPushButton::clicked,
        this,
        &View::createNewGroup
    );
    connect(
        settingsButton,
        &QPushButton::clicked,
        this,
        &View::settingsButtonClicked
    );
    connect(
        dirButton,
        &QPushButton::clicked,
        this,
        &View::createNewFolder
    );
    connect(
        uploadFileButton,
        &QPushButton::clicked,
        this,
        &View::uploadFile
    );
    connect(
        uploadFolderButton,
        &QPushButton::clicked,
        this,
        &View::uploadFolder
    );
}

View::~View() {

}

void View::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);
    controller->tryLogin();
}

void View::setGroupsButtonsVisible() {
    groupsWidget.show();
    filesWidget.hide();
}

void View::setFilesButtonsVisible() {
    groupsWidget.hide();
    filesWidget.show();
}

void View::setSettingsButtonEnabled(bool enabled) {
    settingsButton->setEnabled(enabled);
}

void View::setUploadProgress(qint64 current, qint64 total) {
    uploadProgress->setMinimum(0);
    uploadProgress->setMaximum(total);
    uploadProgress->setValue(current);
}

void View::setDownloadProgress(qint64 current, qint64 total) {
    downloadProgress->setMinimum(0);
    downloadProgress->setMaximum(total);
    downloadProgress->setValue(current);
}

void View::showLogoutMsg() {
    QMessageBox msgBox(this);
    msgBox.setText("You have been logged out!");
    msgBox.exec();
}

void View::showForbiddenMsg() {
    QMessageBox msgBox(this);
    msgBox.setText("You don't have access to this group!");
    msgBox.exec();
}

void View::showUnknownErrorMsg() {
    QMessageBox msgBox;
    msgBox.setText("Error while connecting to the server!");
    msgBox.setInformativeText("Try again. If error persists contact developer.");
    msgBox.exec();
}

QString View::getNewGroupName(bool *ok) {
    return QInputDialog::getText(
        this, "Create new group", "Group name:",
        QLineEdit::Normal, "", ok
    );
}

QString View::getNewFolderName(bool *ok) {
    return QInputDialog::getText(
        this, "New folder", "Name:",
        QLineEdit::Normal, "", ok
    );
}

QStringList View::getUploadFileNames() {
    return QFileDialog::getOpenFileNames(
        this, "Select one or more files to upload", QDir::homePath()
    );
}

QString View::getUploadFolderName() {
    return QFileDialog::getExistingDirectory(
        this, "Select directory to upload", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
}

QString View::getSaveFilename(QString suggested) {
    return QFileDialog::getSaveFileName(this, "Save file", suggested);
}
