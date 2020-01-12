#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/uploaditem.h"

class UploadItemTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testGetters();
};

void UploadItemTest::testGetters() {
    UploadItem item("root/local", "root/server", "relative/path.jpg");

    QCOMPARE(item.getRootLocal(), QString("root/local"));
    QCOMPARE(item.getRootServer(), QString("root/server"));

    QCOMPARE(item.getRelativeDir(), QString("relative/"));
    QCOMPARE(item.getFilename(), QString("path.jpg"));
    QCOMPARE(item.getRelativePath(), QString("relative/path.jpg"));
}

QTEST_APPLESS_MAIN(UploadItemTest)

#include "tst_uploaditemtest.moc"
