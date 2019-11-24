#include <QApplication>

#include "mainwindow.h"
#include "controller.h"
#include "model.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainWindow view;
    Model model;
    Controller controller;

    controller.setModel(&model);
    controller.setView(&view);
    view.setController(&controller);
    controller.showGroups();

    view.show();

    return a.exec();
}
