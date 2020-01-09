#include <QApplication>

#include "include/webbridge.h"
#include "include/mainwindow.h"
#include "include/controller.h"
#include "include/model.h"
#include "include/rabbitconfig.h"
#include "include/rabbitreceiver.h"
#include "include/rabbitmqsecrets.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    WebBridge bridge("https://elkadrive.herokuapp.com");
    RabbitConfig rabbitConfig(
        RABBITMQ_HOST,
        RABBITMQ_VHOST,
        RABBITMQ_USERNAME,
        RABBITMQ_PASSWORD,
        RABBITMQ_PORT
    );
    RabbitReceiver rabbitReceiver(rabbitConfig);

    MainWindow view;
    Model model(&bridge, &rabbitReceiver);
    Controller controller(&app, &view, &model);

    view.setController(&controller);

    view.show();

    return app.exec();
}
