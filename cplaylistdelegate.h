#ifndef CPLAYLISTDELEGATE_H
#define CPLAYLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <memory>
#include <QLineEdit>
#include <QMargins>
#include <QVBoxLayout>
#include "cplaylistsmodel.h"

/*
 * Class representing playlist delegate.
 * Allows editing playlist title and description.
 * Embraces internal editor widget class.
 * First attempt ever to create delegate with editor :)
 */

class CPlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT
private:
    /*
     * Private nested class EditorWidget
     * - definition moved outside class because of "QT moc not supporting nested classes"
     * see definition below
     */
    class playlistEditorWidget;

    enum fontType {titleFont, descriptionFont, statusFont};

    static constexpr QSize itemSize = QSize(100, 50);
    static constexpr QMargins margin = QMargins(2, 2, 2, 2);

public:
    // def ctor
    CPlaylistDelegate(QWidget* parent = 0);

    // overriden
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

/*
 * NESTED EDITORWIDGET CLASS, THAT EMBRACES EDITOR WIDGETS
 */

class CPlaylistDelegate::playlistEditorWidget : public QWidget
{
    Q_OBJECT
private:
    // edit widgets
    std::unique_ptr<QLineEdit> title_ptr, descr_ptr;
    // string for keeping fragments q
    QString statusTip;
    // fonts
    const QFont titleFont, descriptionFont, statusFont;
    // another display features
    const QStyleOptionViewItem option;
    const QMargins margins;

public:
    playlistEditorWidget(const QFont& title, const QFont& desc, const QFont& status, const QStyleOptionViewItem &opt, const QMargins margin, QWidget* parent = 0);
    void setData(const QString& title, const QString& description, const QString& status);
    // overriden
    void paintEvent(QPaintEvent* event) override;

    // getters
    QString getTitle() const
    {
        return title_ptr->text();
    }
    QString getDescription() const
    {
        return descr_ptr->text();
    }

};

#endif // CPLAYLISTDELEGATE_H
