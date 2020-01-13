#ifndef MOCKRECEIVER_H
#define MOCKRECEIVER_H

#include "include/messagereceiver.h"

class MockReceiver : public MessageReceiver {
public:
    ~MockReceiver() override;

    void start() override;
    void setUser(const QString &user) override;
    void connectOnlyGivenGroups(const QList<int> &groups) override;
    void connectGroup(const int groupId) override;
    void disconnectGroup(const int groupId) override;

    void emitMsg(const int groupId, const Message &msg);
};

#endif // MOCKRECEIVER_H
