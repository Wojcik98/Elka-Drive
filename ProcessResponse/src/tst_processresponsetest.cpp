#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/model.h"
#include "include/mockbridge.h"

class ProcessResponseTest : public QObject {
    Q_OBJECT

public:
    ProcessResponseTest();

private:
    Model *model;
    MockBridge bridge;

    QString user = "user";
    QString password = "pwd";

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testLoginCorrect();
    void testLoginFailed();
};

ProcessResponseTest::ProcessResponseTest() {
}

void ProcessResponseTest::initTestCase() {
    model = new Model(&bridge);
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
