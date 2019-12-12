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
    void testLogin_data();
    void testLogin();
};

ProcessResponseTest::ProcessResponseTest() {
}

void ProcessResponseTest::initTestCase() {
    model = new Model(&bridge);
}

void ProcessResponseTest::cleanupTestCase() {
    delete model;
}

void ProcessResponseTest::testLogin_data() {
    QTest::addColumn<QString>("responseBody");
    QTest::addColumn<int>("responseStatus");
    QTest::addColumn<bool>("expected");

    QTest::newRow("Correct login") << "" << 302 << true;
    QTest::newRow("Failed login") << "" << 401 << false;
}

void ProcessResponseTest::testLogin() {
    QSignalSpy spy(model, &Model::loginStatus);

    QFETCH(QString, responseBody);
    QFETCH(int, responseStatus);
    QFETCH(bool, expected);

    auto response = Response(responseStatus, responseBody.toUtf8(), Response::Type::LOGIN);
    bridge.setResponse(response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toBool(), expected);
}

QTEST_APPLESS_MAIN(ProcessResponseTest)

#include "tst_processresponsetest.moc"
