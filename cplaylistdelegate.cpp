#include "cplaylistdelegate.h"

CPlaylistDelegate::CPlaylistDelegate(QWidget* parent)
{
    Q_UNUSED(parent);
}

void CPlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    // obtaining data
    const auto& title = index.data(Qt::DisplayRole).toString();
    const auto& description = index.data(Qt::ToolTipRole).toString();
    const auto& size = index.data(Qt::StatusTipRole).toString();
    const auto& align = index.data(Qt::TextAlignmentRole).value<Qt::Alignment>();
    auto font = index.data(Qt::FontRole).value<QFont>();
    qDebug() << "Painting: " << title << " " << description;

    // drawing
    // background
    painter->fillRect(option.rect, (option.state & QStyle::State_Selected) ? option.palette.highlight() : option.palette.background());
    auto rect = option.rect.translated(3, 0);

    // title
    painter->save();
    font.setPointSize(12);
    font.setBold(true);
    painter->setFont(font);
    QApplication::style()->drawItemText(painter, rect, align, option.palette, true, title);
    painter->restore();

    // description
    font.setPointSize(10);
    font.setBold(false);
    font.setItalic(true);
    painter->setFont(font);
    rect.translate(0, 17);
    QApplication::style()->drawItemText(painter, rect, align, option.palette, true, description);

    // fragments quantity
    rect.translate(0, 15);
    font.setItalic(false);
    painter->setFont(font);
    QApplication::style()->drawItemText(painter, rect, align, option.palette, true, size);
}

QSize CPlaylistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    return QSize(100, 50);
}
