#include <QApplication>

#include "include/webbridge.h"
#include "include/mainwindow.h"
#include "include/controller.h"
#include "include/model.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    WebBridge bridge("https://elkadrive.herokuapp.com");

    MainWindow view;
    Model model(&bridge);
    Controller controller;

    controller.setModel(&model);
    controller.setView(&view);
    view.setController(&controller);
    controller.showGroups();

    view.show();
    controller.checkLogin();

    return a.exec();
}
