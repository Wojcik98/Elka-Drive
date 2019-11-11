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
    qDebug() << "clicked: " << index.data().toString();

    if (index.data(Qt::UserRole + 1).toString() == "folder" || index.data(Qt::UserRole + 1).toString() == "group") {
        path.append(index.data().toString());
        qDebug() << "get " << path.join("/");
        auto result = model->getPath(path.join("/"));
        view->setFileList(result);
    } else {
        qDebug() << "Download!";
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
        auto groups = model->getGroups();
        view->setFileList(groups);
    }
}
