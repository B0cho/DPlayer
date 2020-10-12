#ifndef CSEARCHLISTDELEGATE_H
#define CSEARCHLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

/// Read-only delegate, displaying following information:\n
/// Title [Qt::DisplayRole] [Calibri, 12pt]\n
/// Description [Qt::TooltipRole] / Additional info [Qt::StatusTipRole] [Calibri, 10pt]
class CSearchListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CSearchListDelegate(QWidget* parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CSEARCHLISTDELEGATE_H
