#ifndef CFRAGMENTDELEGATE_H
#define CFRAGMENTDELEGATE_H

#include <QStyledItemDelegate>
#include <QObject>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QMargins>
#include <QLineEdit>
#include <memory>
#include <QVBoxLayout>

/*
 * Class representing fragment delegate.
 *
 *
 *
 */

class CFragmentDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    // default item properties
    static constexpr QSize itemSize{100, 50};
    static constexpr QMargins itemMargins = QMargins(2, 2, 2, 2);

public:
    CFragmentDelegate(QWidget* parent = 0);

    // overriden
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // CFRAGMENTDELEGATE_H
