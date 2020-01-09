#include "include/rabbitreceiver.h"

RabbitReceiver::RabbitReceiver(RabbitConfig config, QObject *parent) : MessageReceiver(parent) {
    client.setAutoReconnect(true);
    client.setHost(config.getHost());
    client.setVirtualHost(config.getVhost());
    client.setUsername(config.getUsername());
    client.setPassword(config.getPassword());
    client.setPort(config.getPort());
}

RabbitReceiver::~RabbitReceiver() {

}

void RabbitReceiver::setUser(QString user) {
    this->user = user;
}

void RabbitReceiver::start() {
    connect(
        &client,
        &QAmqpClient::connected,
        this,
        &RabbitReceiver::clientConnected
    );
    client.connectToHost();
}

void RabbitReceiver::clientConnected() {
    connectedToHost = true;
}

void RabbitReceiver::connectGroup(int groupId) {
    if (queueToGroup.values().contains(groupId)) {
        return;
    }

    QString queueName = user + "." + QString::number(groupId);
    QAmqpQueue *queue = client.createQueue(queueName);
    queues.append(queue);
    queueToGroup[queue] = groupId;

    disconnect(queue, nullptr, nullptr, nullptr); // in case this is a reconnect
    connect(
        queue,
        &QAmqpQueue::declared,
        this,
        &RabbitReceiver::groupConnected
    );

    queue->declare(queueOptions);
}

void RabbitReceiver::groupConnected() {
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue) {
        // bad cast
        return;
    }

    connect(
        queue,
        &QAmqpQueue::messageReceived,
        this,
        &RabbitReceiver::rawMessageReceived
    );

    queue->consume(QAmqpQueue::coNoAck);
}

void RabbitReceiver::rawMessageReceived() {
    QAmqpQueue *queue = qobject_cast<QAmqpQueue*>(sender());
    if (!queue) {
        // bad cast
        return;
    }

    QAmqpMessage message = queue->dequeue();
    QByteArray content = message.payload();
    int groupId = queueToGroup[queue];
    Message msg("", content);

    emit messageReceived(groupId, msg);
}
