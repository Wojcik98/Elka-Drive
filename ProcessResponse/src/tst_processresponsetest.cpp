#include <QString>
#include <QtTest>
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
    void testNoKey();
    void testWrongType();
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
    QTest::addColumn<QString>("response");
    QTest::addColumn<bool>("expected");

    QTest::newRow("Correct login") << "{\"success\": true}" << true;
    QTest::newRow("Failed login") << "{\"success\": false}" << false;
}

void ProcessResponseTest::testLogin() {
    QFETCH(QString, response);
    QFETCH(bool, expected);

    auto json = QJsonDocument::fromJson(response.toUtf8()).object();
    bridge.setResponse(json);
    bool success = model->requestLogin(user, password);
    QCOMPARE(success, expected);
}

void ProcessResponseTest::testNoKey() {
    QString response = "{\"success\": 3.451}";
    auto json = QJsonDocument::fromJson(response.toUtf8()).object();
    bridge.setResponse(json);
    QVERIFY_EXCEPTION_THROWN(model->requestLogin(user, password), std::invalid_argument);
}

void ProcessResponseTest::testWrongType() {
    QString response = "{\"other\": true}";
    auto json = QJsonDocument::fromJson(response.toUtf8()).object();
    bridge.setResponse(json);
    QVERIFY_EXCEPTION_THROWN(model->requestLogin(user, password), std::invalid_argument);
}

QTEST_APPLESS_MAIN(ProcessResponseTest)

#include "tst_processresponsetest.moc"
