#include "csearchlistdelegate.h"

CSearchListDelegate::CSearchListDelegate(QWidget *parent):
    QStyledItemDelegate(parent)
{

}

void CSearchListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    // data
    const auto& title = index.data(Qt::DisplayRole).toString();
    const auto& description = index.data(Qt::ToolTipRole).toString() + " / " + index.data(Qt::StatusTipRole).toString();

    QFont titleFont("Calibri"), descriptionFont("Calibri");
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    descriptionFont.setPointSize(10);
    // drawing
    // background
    painter->fillRect(option.rect, (option.state & QStyle::State_Selected) ? option.palette.highlight() : QApplication::palette().window());
    auto rect = option.rect.marginsRemoved(QMargins(2, 2, 2, 2));

    // title
    painter->setFont(titleFont);
    QApplication::style()->drawItemText(painter, rect, Qt::AlignLeft | Qt::AlignTop, option.palette, true, title);

    // description
    painter->setFont(descriptionFont);
    QApplication::style()->drawItemText(painter, rect, Qt::AlignLeft | Qt::AlignBottom, option.palette, true, description);
}

QSize CSearchListDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 50);
    Q_UNUSED(option);
    Q_UNUSED(index);
}
