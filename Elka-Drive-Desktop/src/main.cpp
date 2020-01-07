#include <QApplication>

#include "include/webbridge.h"
#include "include/mainwindow.h"
#include "include/controller.h"
#include "include/model.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WebBridge bridge("https://elkadrive.herokuapp.com");

    MainWindow view;
    Model model(&bridge);
    Controller controller(&app, &view, &model);

    view.setController(&controller);

    view.show();

    return app.exec();
}
