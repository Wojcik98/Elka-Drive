#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/model.h"
#include "include/mockbridge.h"
#include "include/rabbitconfig.h"
#include "include/rabbitreceiver.h"

class ProcessResponseTest : public QObject {
    Q_OBJECT

public:
    ProcessResponseTest();
    ~ProcessResponseTest();

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

ProcessResponseTest::ProcessResponseTest() {
    RabbitConfig rabbitConfig("", "", "", "", 0);
    rabbitReceiver = new RabbitReceiver(rabbitConfig);
}

ProcessResponseTest::~ProcessResponseTest() {
    delete rabbitReceiver;
}

void ProcessResponseTest::initTestCase() {
    model = new Model(&bridge, rabbitReceiver);
}

void ProcessResponseTest::cleanupTestCase() {
    delete model;
}

void ProcessResponseTest::testLoginCorrect() {
    QSignalSpy spy(model, &Model::userLogged);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitGotResponse(response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);
}

void ProcessResponseTest::testLoginFailed() {
    QSignalSpy spy(model, &Model::responseError);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitResponseError(QNetworkReply::AuthenticationRequiredError, response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);
}

QTEST_APPLESS_MAIN(ProcessResponseTest)

#include "tst_processresponsetest.moc"
