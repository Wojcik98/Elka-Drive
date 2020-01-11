#include "include/messagedelegate.h"
#include "include/message.h"
#include <QApplication>
#include <QDebug>

MessageDelegate::MessageDelegate(QObject *parent)
    : QStyledItemDelegate(parent) {

}

void MessageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    painter->save();
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    painter->setClipping(true);
    painter->setClipRect(rect);
    painter->setFont(opt.font);
    painter->fillRect(rect, palette.light().color());

    int userRight = drawUser(painter, option, index);
    int headerBottom = drawTime(painter, option, index, userRight);

    drawMessage(painter, option, index, headerBottom);

    drawLine(painter, option, index);

    painter->restore();
}

int MessageDelegate::drawUser(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    QFont f(opt.font);
    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    QRect userRect = userBox(opt, index);
    userRect.moveTo(0, rect.top());

    f.setPointSizeF(0.6 * opt.font.pointSize());
    f.setBold(true);

    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(
        userRect,
        Qt::TextSingleLine,
        index.data(Message::Role::USER).toString()
    );


    return userRect.right();
}

int MessageDelegate::drawTime(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, int userRight) const {
    QStyleOptionViewItem opt(option);
    QFont f(opt.font);
    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    QRect timestampRect = timestampBox(opt, index);
    timestampRect.moveTo(userRight + MARGIN, rect.top());

    f.setPointSizeF(0.6 * opt.font.pointSize());
    f.setBold(false);

    painter->setFont(f);
    painter->setPen(palette.text().color());
    painter->drawText(
        timestampRect,
        Qt::TextSingleLine,
        index.data(Message::Role::TIMESTAMP).toString()
    );

    return timestampRect.bottom();
}

void MessageDelegate::drawMessage(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index, int top) const {
    QStyleOptionViewItem opt(option);
    const QPalette &palette(opt.palette);

    QRect messageRect = messageBox(option, index);

    messageRect.moveTo(0, top);

    painter->setFont(opt.font);
    painter->setPen(palette.windowText().color());
    painter->drawText(messageRect, Qt::TextWordWrap, index.data(Message::Role::TEXT).toString());
}

void MessageDelegate::drawLine(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex&) const {
    QStyleOptionViewItem opt(option);
    const QPalette &palette(opt.palette);
    const QRect &rect(opt.rect);

    painter->setPen(palette.dark().color());
    painter->drawLine(rect.left(), rect.bottom(), rect.right(), rect.bottom());
}

QRect MessageDelegate::userBox(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QFont f(option.font);
    f.setPointSizeF(0.6 * f.pointSize());
    f.setBold(true);

    auto userBox = QFontMetrics(f)
            .boundingRect(index.data(Message::Role::USER).toString())
            .adjusted(0, 0, 1, 1);

    return userBox;
}

QRect MessageDelegate::timestampBox(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QFont f(option.font);
    f.setPointSizeF(0.6 * f.pointSize());
    auto timestampBox = QFontMetrics(f)
            .boundingRect(index.data(Message::Role::TIMESTAMP).toString())
            .adjusted(0, 0, MARGIN, 1);

    return timestampBox;
}

QRect MessageDelegate::messageBox(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    auto rect = QRect(option.rect.x(), option.rect.y(), option.rect.width(), 1000);
    QFont f(option.font);

    return QFontMetrics(f)
            .boundingRect(
                rect,
                Qt::TextWordWrap,
                index.data(Message::Role::TEXT).toString())
            .adjusted(0, 0, 1, 1);
}

QSize MessageDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    auto height = userBox(opt, index).height() + messageBox(opt, index).height();

    return QSize(opt.rect.width(), height);
}
