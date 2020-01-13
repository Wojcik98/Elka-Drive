#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/model.h"
#include "include/mockbridge.h"
#include "include/mockreceiver.h"

Q_DECLARE_METATYPE(QList<QStandardItem*>)

class ModelTest : public QObject {
    Q_OBJECT

public:
    ModelTest();

private:
    Model *model;
    MockBridge bridge;
    MockReceiver receiver;
    QStandardItemModel standardModel;
    QStandardItem fileItem;
    QStandardItem dirItem;

    QString user = "user";
    QString password = "pwd";

private Q_SLOTS:
    void init();
    void cleanup();
    void testLoginCorrect();
    void testRegisterCorrect();
    void testGroupsCorrect();
    void testDelete();
    void testDownload();
    void testPath();
    void testNewGroup();
    void testGroupDelete();
    void testGroupUsers();
    void testAddUserToGroup();
    void testRemoveUserFromGroup();
    void testNewFolder();
    void testFileUpload();
    void testBack();
    void testRefresh();
    void testSendMsg();
    void testErrorResponse();
};

ModelTest::ModelTest() : fileItem("filename"), dirItem("dirname") {
    qRegisterMetaType<QList<QStandardItem*>>("QList<QStandardItem*>");

    fileItem.setData(QVariant(0), Model::ID_ROLE);
    fileItem.setData(QVariant(Model::ItemType::FILE), Model::TYPE_ROLE);
    standardModel.appendRow(&fileItem);

    dirItem.setData(QVariant(1), Model::ID_ROLE);
    dirItem.setData(QVariant(Model::ItemType::FOLDER), Model::TYPE_ROLE);
    standardModel.appendRow(&dirItem);
}

void ModelTest::init() {
    model = new Model(&bridge, &receiver);
}

void ModelTest::cleanup() {
    delete model;
    receiver.connectedGroups.clear();
}

void ModelTest::testLoginCorrect() {
    QSignalSpy spy(model, &Model::userLogged);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitGotResponse(response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(model->isLogged(), true);
}

void ModelTest::testRegisterCorrect() {
    QSignalSpy spy(model, &Model::userRegistered);

    auto response = Response("", RequestType::REGISTER);
    bridge.emitGotResponse(response);
    model->requestRegister(user, password);
    QCOMPARE(spy.count(), 1);
    QCOMPARE(model->isLogged(), false);
}

void ModelTest::testGroupsCorrect() {
    QSignalSpy spy(model, &Model::groupsReceived);

    QString json = "[{\"id\":0,\"name\":\"first\"}, {\"id\":1,\"name\":\"second\"}]";
    auto response = Response(json.toUtf8(), RequestType::GROUPS);
    bridge.emitGotResponse(response);

    model->requestGroups();

    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    auto groups = qvariant_cast<QList<QStandardItem*>>(arguments.first());
    QCOMPARE(groups.size(), 2);

    auto first = groups[0];
    auto second = groups[1];

    auto firstId = first->data(Model::ID_ROLE).toInt();
    QVERIFY(QList<int>({0, 1}).contains(firstId));
    if (firstId == 1) {
        auto tmp = first;
        first = second;
        second = tmp;
    }
    QCOMPARE(second->data(Model::ID_ROLE).toInt(), 1);

    QCOMPARE(first->data(Model::TYPE_ROLE).toInt(), static_cast<int>(Model::ItemType::GROUP));
    QCOMPARE(second->data(Model::TYPE_ROLE).toInt(), static_cast<int>(Model::ItemType::GROUP));

    QCOMPARE(first->data(Qt::DisplayRole).toString(), QString("first"));
    QCOMPARE(second->data(Qt::DisplayRole).toString(), QString("second"));

    QVERIFY(receiver.connectedGroups.contains(0));
    QVERIFY(receiver.connectedGroups.contains(1));
}

void ModelTest::testDelete() {
    QSignalSpy spy(model, &Model::resourceDeleted);
    Response response("", RequestType::DELETE);
    bridge.emitGotResponse(response);

    model->requestDelete(fileItem.index());
    QCOMPARE(spy.count(), 1);

    model->requestDelete(dirItem.index());
    QCOMPARE(spy.count(), 2);
}

void ModelTest::testDownload() {
    QSignalSpy spy(model, &Model::downloadProgress);
    bridge.emitDownloadProgress(42, 42);
    QString path = "some/path";

    model->requestDownload(fileItem.index(), path);
    QCOMPARE(spy.count(), 1);

    model->requestDownload(dirItem.index(), path);
    QCOMPARE(spy.count(), 2);
}

void ModelTest::testPath() {

}

void ModelTest::testNewGroup() {

}

void ModelTest::testGroupDelete() {

}

void ModelTest::testGroupUsers() {

}

void ModelTest::testAddUserToGroup() {

}

void ModelTest::testRemoveUserFromGroup() {

}

void ModelTest::testNewFolder() {

}

void ModelTest::testFileUpload() {

}

void ModelTest::testBack() {

}

void ModelTest::testRefresh() {

}

void ModelTest::testSendMsg() {

}


void ModelTest::testErrorResponse() {
    QSignalSpy spy(model, &Model::responseError);

    auto response = Response("", RequestType::LOGIN);
    bridge.emitResponseError(QNetworkReply::ContentAccessDenied, response);
    model->requestLogin(user, password);
    bridge.emitResponseError(QNetworkReply::AuthenticationRequiredError, response);
    model->requestLogin(user, password);
    QCOMPARE(spy.count(), 2);
}

QTEST_APPLESS_MAIN(ModelTest)

#include "tst_modeltest.moc"
