#ifndef CSEARCHLISTDELEGATE_H
#define CSEARCHLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class CSearchListDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:

public:
    CSearchListDelegate(QWidget* parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // CSEARCHLISTDELEGATE_H
