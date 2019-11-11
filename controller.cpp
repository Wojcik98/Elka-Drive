#include <QtDebug>

#include "controller.h"

Controller::Controller() {

}

void Controller::setModel(Model *model) {
    this->model = model;
}

void Controller::setView(MainWindow *view) {
    this->view = view;
}

void Controller::showGroups() {
    auto groups = model->getGroups();
    view->setFileList(groups);
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    qDebug() << "clicked: " << index.data().toString() << " " << index.data(Qt::UserRole + 1).toString();

    if (index.data(Qt::UserRole + 1).toString() == "folder" || index.data(Qt::UserRole + 1).toString() == "group") {
        auto result = model->getPath("/");
        view->setFileList(result);
    } else {
        qDebug() << "Download!";
    }
}
