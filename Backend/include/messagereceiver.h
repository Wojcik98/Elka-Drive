#ifndef MESSAGERECEIVER_H
#define MESSAGERECEIVER_H

#include <QObject>
#include "include/message.h"

/*!
 * \brief MessageReceiver Interfejs do odbierania wiadomości z czatu.
 */
class MessageReceiver : public QObject {
Q_OBJECT

public:
    MessageReceiver(QObject *parent = nullptr);
    virtual ~MessageReceiver();

    virtual void start() = 0;
    virtual void setUser(const QString &user) = 0;
    virtual void connectOnlyGivenGroups(const QList<int> &groups) = 0;

signals:
    /*!
     * \brief Emitowany gdy odebrano nową wiadomość.
     * \param groupId ID grupy, do której przyszła wiadomość
     * \param msg Wiadomość
     */
    void messageReceived(const int groupId, const Message &msg);
};

#endif // MESSAGERECEIVER_H
