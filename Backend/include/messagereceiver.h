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
    virtual void setUser(QString user) = 0;
    virtual void connectGroup(int groupId) = 0;

signals:
    void messageReceived(int groupId, Message msg);
};

#endif // MESSAGERECEIVER_H
