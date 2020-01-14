#include "include/view.h"

View::View(QWidget *parent)
    : QMainWindow(parent), groupsWidget(this), filesWidget(this) {
    setupBottomButtons();
    setupUpload();
    setupDownload();

    connectSignals();
}

View::~View() {

}

void View::setupBottomButtons() {
    QHBoxLayout *groupsButtons = new QHBoxLayout();
    newButton = new QPushButton(QIcon(":/icons/add_circle.svg"), "Group", this);
    settingsButton = new QPushButton(QIcon(":/icons/settings.svg"), "Settings", this);
    groupsButtons->addWidget(settingsButton);
    groupsButtons->addWidget(newButton);

    QGridLayout *filesButtons = new QGridLayout();
    dirButton = new QPushButton(QIcon(":/icons/create_new_folder.svg"), "Folder", this);
    uploadFileButton = new QPushButton(QIcon(":/icons/cloud_upload.svg"), "Upload file", this);
    uploadFolderButton = new QPushButton(QIcon(":/icons/cloud_upload.svg"), "Upload folder", this);
    downloadButton = new QPushButton(QIcon(":/icons/cloud_download.svg"), "Download", this);
    deleteButton = new QPushButton(QIcon(":/icons/delete.svg"), "Delete", this);
    filesButtons->addWidget(dirButton, 0, 0);
    filesButtons->addWidget(uploadFolderButton, 0, 1);
    filesButtons->addWidget(uploadFileButton, 0, 2);
    filesButtons->addWidget(downloadButton, 1, 0);
    filesButtons->addWidget(deleteButton, 1, 1);

    groupsWidget.setLayout(groupsButtons);
    groupsWidget.show();
    filesWidget.setLayout(filesButtons);
    filesWidget.hide();
}

void View::setupUpload() {
    uploadWidget = new QWidget(this);
    auto uploadLayout = new QGridLayout(uploadWidget);
    uploadProgress = new QProgressBar(uploadWidget);
    uploadFinishedLabel = new QLabel("Finished!", uploadWidget);
    uploadLayout->addWidget(uploadProgress, 0, 0);
    uploadLayout->addWidget(uploadFinishedLabel, 0, 0);
    uploadProgress->setVisible(true);
    uploadFinishedLabel->setVisible(false);

    uploadTimer.setInterval(3000);
    uploadTimer.setSingleShot(true);
}

void View::setupDownload() {
    downloadWidget = new QWidget(this);
    auto downloadLayout = new QGridLayout(downloadWidget);
    downloadProgress = new QProgressBar(downloadWidget);
    downloadFinishedLabel = new QLabel("Finished!", downloadWidget);
    downloadLayout->addWidget(downloadProgress, 0, 0);
    downloadLayout->addWidget(downloadFinishedLabel, 0, 0);
    downloadProgress->setVisible(true);
    downloadFinishedLabel->setVisible(false);

    downloadTimer.setInterval(3000);
    downloadTimer.setSingleShot(true);
}

void View::connectSignals() {
    connect(
        &uploadTimer,
        &QTimer::timeout,
        this,
        &View::resetUploadProgress
    );
    connect(
        &downloadTimer,
        &QTimer::timeout,
        this,
        &View::resetDownloadProgress
    );
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
        downloadButton,
        &QPushButton::clicked,
        this,
        &View::downloadButtonClicked
    );
    connect(
        deleteButton,
        &QPushButton::clicked,
        this,
        &View::deleteButtonClicked
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

void View::setGroupsView() {
    groupsWidget.show();
    filesWidget.hide();
    sendButton->setEnabled(false);
    backButton->setEnabled(false);
}

void View::setFilesView() {
    groupsWidget.hide();
    filesWidget.show();
    sendButton->setEnabled(true);
    backButton->setEnabled(true);
}

void View::anyItemSelected(bool enabled) {
    settingsButton->setEnabled(enabled);
    downloadButton->setEnabled(enabled);
    deleteButton->setEnabled(enabled);
}

void View::setUploadProgress(qint64 current, qint64 total) {
    if (current == total && current != -1) {
        uploadTimer.start();
        uploadProgress->reset();
        uploadProgress->setVisible(false);
        uploadFinishedLabel->setVisible(true);
    } else {
        uploadTimer.stop();
        uploadProgress->setVisible(true);
        uploadFinishedLabel->setVisible(false);

        uploadProgress->setMinimum(0);
        uploadProgress->setMaximum(static_cast<int>(total));
        uploadProgress->setValue(static_cast<int>(current));
    }
}

void View::resetUploadProgress() {
    uploadProgress->reset();
    uploadProgress->setVisible(true);
    uploadFinishedLabel->setVisible(false);
}

void View::setDownloadProgress(qint64 current, qint64 total) {
    if (current == total && current != -1) {
        downloadTimer.start();
        downloadProgress->reset();
        downloadProgress->setVisible(false);
        downloadFinishedLabel->setVisible(true);
    } else {
        downloadTimer.stop();
        downloadProgress->setVisible(true);
        downloadFinishedLabel->setVisible(false);

        downloadProgress->setMinimum(0);
        downloadProgress->setMaximum(static_cast<int>(total));
        downloadProgress->setValue(static_cast<int>(current));
    }
}

void View::resetDownloadProgress() {
    downloadProgress->reset();
    downloadProgress->setVisible(true);
    downloadFinishedLabel->setVisible(false);
}
