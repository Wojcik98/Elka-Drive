#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/model.h"
#include "include/mockbridge.h"
#include "include/rabbitconfig.h"
#include "include/rabbitreceiver.h"

class ModelTest : public QObject {
    Q_OBJECT

public:
    ModelTest();
    ~ModelTest();

private:
    Model *model;
    MockBridge bridge;
    RabbitReceiver *rabbitReceiver; // TODO make mock

    QString user = "user";
    QString password = "pwd";

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testLoginCorrect();
    void testLoginFailed();
};

ModelTest::ModelTest() {
    RabbitConfig rabbitConfig("", "", "", "", 0);
    rabbitReceiver = new RabbitReceiver(rabbitConfig);
}

ModelTest::~ModelTest() {
    delete rabbitReceiver;
}

void ModelTest::initTestCase() {
    model = new Model(&bridge, rabbitReceiver);
}

void ModelTest::cleanupTestCase() {
    delete model;
}

void ModelTest::testLoginCorrect() {
    QSignalSpy spy(model, &Model::userLogged);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitGotResponse(response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testLoginFailed() {
    QSignalSpy spy(model, &Model::responseError);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitResponseError(QNetworkReply::AuthenticationRequiredError, response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(ModelTest)

#include "tst_modeltest.moc"
