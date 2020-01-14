#ifndef MESSAGEDELEGATE_H
#define MESSAGEDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>

/*!
 * \brief MessageDelegate rysuje wiadomość z czatu.
 */
class MessageDelegate : public QStyledItemDelegate {
public:
    MessageDelegate(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    const int MARGIN = 10;

    int drawUser(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    int drawTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, int right) const;
    void drawMessage(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, int top) const;
    void drawLine(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QRect userBox(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QRect timestampBox(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QRect messageBox(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // MESSAGEDELEGATE_H
