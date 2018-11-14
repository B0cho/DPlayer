#ifndef CPLAYLISTDELEGATE_H
#define CPLAYLISTDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>
/*
 * PLAYLIST DELEGATE
 */
class CPlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CPlaylistDelegate(QWidget* parent = 0);

    /// overriden
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};

#endif // CPLAYLISTDELEGATE_H
