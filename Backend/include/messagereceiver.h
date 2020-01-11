#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>
#include "include/message.h"

class MessageReceiver : public QObject {
Q_OBJECT

public:
    MessageReceiver(QObject *parent = nullptr);
    virtual ~MessageReceiver();

    virtual void start() = 0;
    virtual void setUser(const QString &user) = 0;
    virtual void connectOnlyGivenGroups(const QList<int> &groups) = 0;
    virtual void connectGroup(const int groupId) = 0;
    virtual void disconnectGroup(const int groupId) = 0;

signals:
    void messageReceived(const int groupId, const Message &msg);
};

#endif // MESSAGERECEIVER_H
