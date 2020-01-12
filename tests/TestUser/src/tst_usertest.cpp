#include <QString>
#include <QtTest>
#include <QPair>
#include "include/user.h"

class UserTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testUserGetters();
    void testMakeFromJson();
};

void UserTest::testUserGetters() {
    User user("user", 123);

    QCOMPARE(user.getId(), 123);
    QCOMPARE(user.getName(), QString("user"));
}

void UserTest::testMakeFromJson() {
    auto json = QJsonObject({
        qMakePair(QString("username"), QJsonValue("user")),
        qMakePair(QString("id"), QJsonValue(123))
    });
    auto user = User::fromJson(json);

    QCOMPARE(user.getId(), 123);
    QCOMPARE(user.getName(), QString("user"));
}

QTEST_APPLESS_MAIN(UserTest)

#include "tst_usertest.moc"
