#include "include/view.h"

View::View(QWidget *parent) : QMainWindow(parent), groupsWidget(this), filesWidget(this) {
    QHBoxLayout *groupsButtons = new QHBoxLayout();
    newButton = new QPushButton(QIcon(":/icons/add_circle.svg"), "New", this);
    settingsButton = new QPushButton(QIcon(":/icons/settings.svg"), "Settings", this);
    groupsButtons->addWidget(settingsButton);
    groupsButtons->addWidget(newButton);

    QHBoxLayout *filesButtons = new QHBoxLayout();
    dirButton = new QPushButton(QIcon(":/icons/create_new_folder.svg"), "New folder", this);
    uploadButton = new QPushButton(QIcon(":/icons/cloud_upload.svg"), "Upload", this);
    filesButtons->addWidget(dirButton);
    filesButtons->addWidget(uploadButton);

    groupsWidget.setLayout(groupsButtons);
    groupsWidget.show();
    filesWidget.setLayout(filesButtons);
    filesWidget.hide();
}

View::~View() {

}

void View::showEvent(QShowEvent *ev) {
    QMainWindow::showEvent(ev);
    controller->checkLogin();
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
