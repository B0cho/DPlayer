#include "cfragmentdelegate.h"

CFragmentDelegate::CFragmentDelegate(QWidget *parent)
{
    Q_UNUSED(parent);
}

void CFragmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid()) return;

    // obtaining data
    const auto& title = index.data(Qt::DisplayRole).toString();
    const auto& file = index.data(Qt::ToolTipRole).toString();
    const auto& scope = index.data(Qt::StatusTipRole).toString();
    qDebug() << "Painting: " << title;

}

QSize CFragmentDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{

}
