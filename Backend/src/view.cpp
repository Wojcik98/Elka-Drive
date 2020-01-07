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

    uploadWidget = new QWidget(this);
    auto uploadLayout = new QGridLayout(uploadWidget);
    uploadProgress = new QProgressBar(uploadWidget);
    uploadFinishedLabel = new QLabel("Finished!", uploadWidget);
    uploadLayout->addWidget(uploadProgress, 0, 0);
    uploadLayout->addWidget(uploadFinishedLabel, 0, 0);
    uploadProgress->setVisible(true);
    uploadFinishedLabel->setVisible(false);

    downloadWidget = new QWidget(this);
    auto downloadLayout = new QGridLayout(downloadWidget);
    downloadProgress = new QProgressBar(downloadWidget);
    downloadFinishedLabel = new QLabel("Finished!", downloadWidget);
    downloadLayout->addWidget(downloadProgress, 0, 0);
    downloadLayout->addWidget(downloadFinishedLabel, 0, 0);
    downloadProgress->setVisible(true);
    downloadFinishedLabel->setVisible(false);

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
    if (current == total && current != -1) {
        uploadProgress->reset();
        uploadProgress->setVisible(false);
        uploadFinishedLabel->setVisible(true);
    } else {
        uploadProgress->setVisible(true);
        uploadFinishedLabel->setVisible(false);

        uploadProgress->setMinimum(0);
        uploadProgress->setMaximum(static_cast<int>(total));
        uploadProgress->setValue(static_cast<int>(current));
    }
}

void View::setDownloadProgress(qint64 current, qint64 total) {
    if (current == total && current != -1) {
        downloadProgress->reset();
        downloadProgress->setVisible(false);
        downloadFinishedLabel->setVisible(true);
    } else {
        downloadProgress->setVisible(true);
        downloadFinishedLabel->setVisible(false);

        downloadProgress->setMinimum(0);
        downloadProgress->setMaximum(static_cast<int>(total));
        downloadProgress->setValue(static_cast<int>(current));
    }
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

void View::showFileOpenError(const QString &filename) {
    QMessageBox msgBox;
    msgBox.setText("Could not open file!");
    msgBox.setInformativeText(filename);
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
