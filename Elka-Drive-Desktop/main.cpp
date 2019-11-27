#include <QApplication>

#include "webbridge.h"
#include "mainwindow.h"
#include "controller.h"
#include "model.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WebBridge bridge;

    MainWindow view;
    Model model(&bridge);
    Controller controller;

    controller.setModel(&model);
    controller.setView(&view);
    view.setController(&controller);
    controller.showGroups();

    view.show();

    return a.exec();
}
