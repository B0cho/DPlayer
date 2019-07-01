#ifndef CPLAYLISTDELEGATE_H
#define CPLAYLISTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <memory>
#include <QLineEdit>
#include <QMargins>
#include <QVBoxLayout>
#include <QLabel> //
#include "cplaylistsmodel.h"

/*
 * PLAYLIST DELEGATE
 */
/**
 * @brief The CPlaylistDelegate class
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
    /*
     *
     */
    enum fontType {titleFont, descriptionFont, statusFont};
    const QFont getFont(const QFont& font, fontType type) const;

    /*
     *
     */
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

};

/*
 * NESTED EDITORWIDGET CLASS, THAT EMBRACES EDITOR WIDGETS
 */
/**
 * @brief The CPlaylistDelegate::EditorWidget class
 */

class CPlaylistDelegate::playlistEditorWidget : public QWidget
{
    Q_OBJECT
private:
    std::unique_ptr<QLineEdit> title_ptr, descr_ptr;
    QString statusTip;
    const QFont titleFont, descriptionFont, statusFont;
    const QStyleOptionViewItem option;
    const QMargins margins;

public:
    playlistEditorWidget(const QFont& title, const QFont& desc, const QFont& status, const QStyleOptionViewItem &opt, const QMargins margin, QWidget* parent = 0);
    void setData(const QString& title, const QString& description, const QString& status);
    // overriden
    void paintEvent(QPaintEvent* event) override;

};

#endif // CPLAYLISTDELEGATE_H
