#include "include/mockreceiver.h"

MockReceiver::~MockReceiver() {

}

void MockReceiver::start() {

}

void MockReceiver::setUser(const QString &) {

}

void MockReceiver::connectOnlyGivenGroups(const QList<int> &) {

}

void MockReceiver::connectGroup(const int) {

}

void MockReceiver::disconnectGroup(const int) {

}

void MockReceiver::emitMsg(const int groupId, const Message &msg) {
    emit messageReceived(groupId, msg);
}
