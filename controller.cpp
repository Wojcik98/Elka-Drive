#include <QtDebug>

#include "controller.h"

Controller::Controller() {

}

void Controller::setModel(Model *model) {
    this->model = model;
}

void Controller::setView(MainWindow *view) {
    this->view = view;
    view->setFileList(QStringList("Test"));
}

void Controller::fileDoubleClicked(const QModelIndex &index) {
    qDebug() << "click!";
}
