#include <QString>
#include <QtTest>
#include "model.h"

class ProcessResponseTest : public QObject {
    Q_OBJECT

public:
    ProcessResponseTest();

private:
    Model model;

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testLogin_data();
    void testLogin();
};

ProcessResponseTest::ProcessResponseTest() {
}

void ProcessResponseTest::initTestCase() {
}

void ProcessResponseTest::cleanupTestCase() {
}

void ProcessResponseTest::testLogin_data() {
    QTest::addColumn<QString>("data");
    QTest::newRow("0") << QString();
}

void ProcessResponseTest::testLogin() {
    QFETCH(QString, data);  // TODO write own test
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(ProcessResponseTest)

#include "tst_processresponsetest.moc"
