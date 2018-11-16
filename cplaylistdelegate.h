#ifndef CPLAYLISTDELEGATE_H
#define CPLAYLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include "cplaylistsmodel.h"
/*
 * PLAYLIST DELEGATE
 */
class CPlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CPlaylistDelegate(QWidget* parent = 0);

    // overriden
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // CPLAYLISTDELEGATE_H
