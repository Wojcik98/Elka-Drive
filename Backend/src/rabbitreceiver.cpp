#include "include/rabbitreceiver.h"
#include <QJsonDocument>
#include <QJsonObject>

/*!
 * \brief Konstruktor.
 * \param config Konfiguracja RabbitMQ
 * \param parent Rodzic w strukturze Qt
 */
RabbitReceiver::RabbitReceiver(const RabbitConfig &config, QObject *parent)
    : MessageReceiver(parent) {
    client.setAutoReconnect(true);
    client.setHost(config.getHost());
    client.setVirtualHost(config.getVhost());
    client.setUsername(config.getUsername());
    client.setPassword(config.getPassword());
    client.setPort(config.getPort());
}

RabbitReceiver::~RabbitReceiver() {

}

/*!
 * \param user Nazwa zalogowanego użytkownika
 */
void RabbitReceiver::setUser(const QString &user) {
    this->user = user;
}

/*!
 * \brief Uruchamia komunikację z RabbitMQ.
 */
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

/*!
 * \brief Nasłuchuje wiadomości od żądanych grup i rozłącza się z resztą.
 * \param groups Grupy do nasłuchiwania
 */
void RabbitReceiver::connectOnlyGivenGroups(const QList<int> &groups) {
    // connect to given groups
    for (auto group : groups) {
        connectGroup(group);
    }

    // disconnect from any other
    auto existingGroups = groupToQueue.keys();
    for (auto group : existingGroups) {
        if (!groups.contains(group)) {
            disconnectGroup(group);
        }
    }
}

void RabbitReceiver::connectGroup(const int groupId) {
    if (groupToQueue.keys().contains(groupId)) {
        return;
    }

    QString queueName = user + "." + QString::number(groupId);
    QAmqpQueue *queue = client.createQueue(queueName);
    queueToGroup[queue] = groupId;
    groupToQueue[groupId] = queue;

    disconnect(queue, nullptr, this, nullptr);
    connect(
        queue,
        &QAmqpQueue::declared,
        this,
        &RabbitReceiver::groupConnected
    );

    queue->declare(queueOptions);
}

void RabbitReceiver::disconnectGroup(const int groupId) {
    if (groupToQueue.contains(groupId)) {
        auto queue = groupToQueue[groupId];

        groupToQueue.remove(groupId);
        queueToGroup.remove(queue);

        queue->deleteLater();
    }
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
    QByteArray json = message.payload();
    Message msg = parseJson(json);
    int groupId = queueToGroup[queue];

    emit messageReceived(groupId, msg);
}

Message RabbitReceiver::parseJson(QByteArray json) {
    auto dict = QJsonDocument::fromJson(json).object();
    auto user = dict["sender"].toString();
    auto timeMs = dict["sentAt"].toInt();
    auto time = QDateTime::fromSecsSinceEpoch(timeMs);
    auto text = dict["message"].toString();

    return Message(user, text, time);
}
