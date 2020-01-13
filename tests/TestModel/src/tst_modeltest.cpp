#include <QString>
#include <QtTest>
#include <QSignalSpy>
#include "include/model.h"
#include "include/mockbridge.h"
#include "include/mockreceiver.h"

Q_DECLARE_METATYPE(QList<QStandardItem*>)
Q_DECLARE_METATYPE(QList<User>)

class ModelTest : public QObject {
    Q_OBJECT

public:
    ModelTest();

private:
    Model *model;
    MockBridge bridge;
    MockReceiver receiver;
    QStandardItemModel standardModel;
    QStandardItem groupItem;
    QStandardItem dirItem;
    QStandardItem fileItem;

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
    void testGetPath();
    void testSubpath();
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

ModelTest::ModelTest() : groupItem("groupname"), dirItem("dirname"), fileItem("filename")  {
    qRegisterMetaType<QList<QStandardItem*>>("QList<QStandardItem*>");
    qRegisterMetaType<QList<User>>("QList<User>");

    groupItem.setData(QVariant(0), Model::ID_ROLE);
    groupItem.setData(QVariant(Model::ItemType::GROUP), Model::TYPE_ROLE);
    standardModel.appendRow(&groupItem);

    dirItem.setData(QVariant(1), Model::ID_ROLE);
    dirItem.setData(QVariant(Model::ItemType::FOLDER), Model::TYPE_ROLE);
    standardModel.appendRow(&dirItem);

    fileItem.setData(QVariant(2), Model::ID_ROLE);
    fileItem.setData(QVariant(Model::ItemType::FILE), Model::TYPE_ROLE);
    standardModel.appendRow(&fileItem);
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
    QCOMPARE(first->data(Model::ID_ROLE).toInt(), 0);
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

void ModelTest::testSubpath() {
    QSignalSpy spy(model, &Model::pathReceived);

    QString json = "[{\"id\":0,\"name\":\"directory\",\"dir\":true}, {\"id\":1,\"name\":\"file\",\"dir\":false}]";
    auto response = Response(json.toUtf8(), RequestType::PATH);
    bridge.emitGotResponse(response);

    model->requestSubpath(groupItem.index());
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    auto dir = qvariant_cast<QList<QStandardItem*>>(arguments.first());
    QCOMPARE(dir.size(), 2);

    auto first = dir[0];
    auto second = dir[1];

    auto firstId = first->data(Model::ID_ROLE).toInt();
    QVERIFY(QList<int>({0, 1}).contains(firstId));
    if (firstId == 1) {
        auto tmp = first;
        first = second;
        second = tmp;
    }
    QCOMPARE(first->data(Model::ID_ROLE).toInt(), 0);
    QCOMPARE(first->data(Model::TYPE_ROLE).toInt(), static_cast<int>(Model::ItemType::FOLDER));
    QCOMPARE(first->data(Qt::DisplayRole).toString(), QString("directory"));
    QCOMPARE(second->data(Model::ID_ROLE).toInt(), 1);
    QCOMPARE(second->data(Model::TYPE_ROLE).toInt(), static_cast<int>(Model::ItemType::FILE));
    QCOMPARE(second->data(Qt::DisplayRole).toString(), QString("file"));

    model->requestSubpath(dirItem.index());
    QCOMPARE(spy.count(), 1);
    spy.removeFirst();

    model->requestSubpath(fileItem.index());
    QCOMPARE(spy.count(), 0);
}

void ModelTest::testGetPath() {
    QCOMPARE(model->getPath(), QString("Groups"));

    // request group so model knows mapping <id> -> <name>
    QString jsonGroup = "[{\"id\":0,\"name\":\"group\"}]";
    auto responseGroup = Response(jsonGroup.toUtf8(), RequestType::GROUPS);
    bridge.emitGotResponse(responseGroup);
    model->requestGroups();

    QString json = "[{\"id\":0,\"name\":\"directory\",\"dir\":true}, {\"id\":1,\"name\":\"file\",\"dir\":false}]";
    auto response = Response(json.toUtf8(), RequestType::PATH);
    bridge.emitGotResponse(response);

    model->requestSubpath(groupItem.index());

    QCOMPARE(model->getPath(), QString("group"));

    model->requestSubpath(dirItem.index());
    QCOMPARE(model->getPath(), QString("group/dirname"));

    model->requestSubpath(fileItem.index());
    QCOMPARE(model->getPath(), QString("group/dirname"));

    model->goBack();
    QCOMPARE(model->getPath(), QString("group"));
}

void ModelTest::testNewGroup() {
    QSignalSpy spy(model, &Model::newGroupCreated);

    auto response = Response("", RequestType::NEW_GROUP);
    bridge.emitGotResponse(response);
    model->requestNewGroup("new group");
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testGroupDelete() {
    QSignalSpy spy(model, &Model::groupDeletedReceived);

    auto response = Response("", RequestType::GROUP_DELETE);
    bridge.emitGotResponse(response);
    model->requestGroupDelete(0);
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testGroupUsers() {
    QSignalSpy spy(model, &Model::groupUsersReceived);

    QString json = "{\"users\":[{\"id\":0,\"username\":\"user\"},{\"id\":1,\"username\":\"Jacek\"}]}";
    auto response = Response(json.toUtf8(), RequestType::GROUP_USERS);
    bridge.emitGotResponse(response);

    model->requestGroupUsers(0);
    QCOMPARE(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    auto users = qvariant_cast<QList<User>>(arguments.first());

    QCOMPARE(users.size(), 2);
    auto first = &users[0];
    auto second = &users[1];

    auto firstId = first->getId();
    QVERIFY(QList<int>({0, 1}).contains(firstId));
    if (firstId == 1) {
        auto tmp = first;
        first = second;
        second = tmp;
    }

    QCOMPARE(first->getId(), 0);
    QCOMPARE(first->getName(), QString("user"));
    QCOMPARE(second->getId(), 1);
    QCOMPARE(second->getName(), QString("Jacek"));
}

void ModelTest::testAddUserToGroup() {
    QSignalSpy spy(model, &Model::groupAddUserReceived);

    auto response = Response("", RequestType::GROUP_ADD_USER);
    bridge.emitGotResponse(response);
    model->requestAddUserToGroup("user", 0);
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testRemoveUserFromGroup() {
    QSignalSpy spy(model, &Model::groupRemoveUserReceived);

    auto response = Response("", RequestType::GROUP_REMOVE_USER);
    bridge.emitGotResponse(response);
    model->requestRemoveUserFromGroup("user", 0);
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testNewFolder() {
    QSignalSpy spy(model, &Model::newFolderCreated);

    auto response = Response("", RequestType::NEW_FOLDER);
    bridge.emitGotResponse(response);
    model->requestNewFolder("new folder");
    QCOMPARE(spy.count(), 1);
}

void ModelTest::testFileUpload() {

}

void ModelTest::testBack() {
    QSignalSpy spy(model, &Model::pathReceived);

    QString jsonGroup = "[{\"id\":0,\"name\":\"group\"}]";
    auto responseGroup = Response(jsonGroup.toUtf8(), RequestType::GROUPS);
    bridge.emitGotResponse(responseGroup);
    model->requestGroups();

    QString json = "[{\"id\":0,\"name\":\"directory\",\"dir\":true}, {\"id\":1,\"name\":\"file\",\"dir\":false}]";
    auto response = Response(json.toUtf8(), RequestType::PATH);
    bridge.emitGotResponse(response);

    model->requestSubpath(groupItem.index());
    model->requestSubpath(dirItem.index());
    QCOMPARE(model->getPath(), QString("group/dirname"));

    spy.clear();
    model->goBack();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(model->getPath(), QString("group"));

    spy.clear();
    model->goBack();
    QCOMPARE(spy.count(), 1);
    QCOMPARE(model->getPath(), QString("Groups"));
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
