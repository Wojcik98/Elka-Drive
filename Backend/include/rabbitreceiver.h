#ifndef RABBITRECEIVER_H
#define RABBITRECEIVER_H

#include "qamqpclient.h"
#include "qamqpqueue.h"
#include "include/messagereceiver.h"
#include "include/rabbitconfig.h"

class RabbitReceiver : public MessageReceiver {
public:
    RabbitReceiver(RabbitConfig config, QObject *parent = nullptr);
    ~RabbitReceiver() override;

    void start() override;
    void setUser(QString user) override;
    void connectGroup(int groupId) override;

private:
    const int queueOptions = QAmqpQueue::Durable;
    bool connectedToHost = false;
    QAmqpClient client;
    QList<QAmqpQueue*> queues;
    QMap<QAmqpQueue*, int> queueToGroup;
    QString user;

private slots:
    void clientConnected();
    void groupConnected();
    void rawMessageReceived();
};

#endif // RABBITRECEIVER_H
