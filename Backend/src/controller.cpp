#include <QtDebug>

#include "include/controller.h"

Controller::Controller() {

}

void Controller::setModel(Model *model) {
    this->model = model;
}

void Controller::setView(View *view) {
    this->view = view;
}

void Controller::showGroups() {
    auto groups = model->getGroups();
    view->setFileList(groups);
}

void Controller::requestDownload(const QModelIndex &index) {
    auto filename = index.data().toString();
    qDebug() << "download: " << filename;
    model->requestDownload(path.join("/") + filename);
}

void Controller::requestDelete(const QModelIndex &index) {
    auto filename = index.data().toString();
    model->requestDelete(path.join("/") + filename);
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    auto filename = index.data().toString();
    auto type = index.data(Qt::UserRole + 1).toString();
    qDebug() << "clicked: " << filename;

    if (type == "folder" || type == "group") {
        path.append(filename);
        qDebug() << "get " << path.join("/");
        auto result = model->getPath(path.join("/"));
        view->setFileList(result);
    } else {
        requestDownload(index);
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
