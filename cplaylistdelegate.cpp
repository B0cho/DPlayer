#include "cplaylistdelegate.h"

constexpr QSize CPlaylistDelegate::itemSize;
constexpr QMargins CPlaylistDelegate::margin;

const QFont CPlaylistDelegate::getFont(const QFont &font, CPlaylistDelegate::fontType type) const
{
    QFont newFont(font);

    // size - 12pt for title, 10pt for others
    newFont.setPointSize((type == CPlaylistDelegate::fontType::titleFont) ? 12 : 10);
    // bold - only for title
    newFont.setBold((type == CPlaylistDelegate::fontType::titleFont) ? true : false);
    // italic - only for status
    newFont.setItalic((type == fontType::statusFont) ? true : false);
    return newFont;
}

CPlaylistDelegate::CPlaylistDelegate(QWidget* parent):
    QStyledItemDelegate(parent)
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
    const auto font = index.data(Qt::FontRole).value<QFont>();
    qDebug() << "Painting: " << title << " " << description;


    // drawing
    // background
    painter->fillRect(option.rect, (option.state & QStyle::State_Selected) ? option.palette.highlight() : option.palette.background());
    auto rect = option.rect.marginsRemoved(margin);

    // title
    painter->setFont(getFont(font, titleFont));
    QApplication::style()->drawItemText(painter, rect, align | Qt::AlignTop, option.palette, true, title);

    // fragments description
    painter->setFont(getFont(font, descriptionFont));
    QApplication::style()->drawItemText(painter, rect, align | Qt::AlignVCenter, option.palette, true, description);

    // fragments quantity
    painter->setFont(getFont(font, statusFont));
    QApplication::style()->drawItemText(painter, rect, align | Qt::AlignBottom, option.palette, true, size);
}

QSize CPlaylistDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    Q_UNUSED(option);
    return itemSize;
}

QWidget *CPlaylistDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const auto font = index.data(Qt::FontRole).value<QFont>();

    // put alignment inside option
    // give margins
    return new playlistEditorWidget(getFont(font, titleFont), getFont(font, descriptionFont), getFont(font, statusFont), option, margin, parent);
}

void CPlaylistDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // casting
    auto editorWidget = qobject_cast<playlistEditorWidget*>(editor);
    if(editorWidget == nullptr)
        return;

    // obtaining data
    const QString title = index.data(Qt::DisplayRole).toString();
    const QString description = index.data(Qt::ToolTipRole).toString();
    const QString status = index.data(Qt::StatusTipRole).toString();

    // setting data
    editorWidget->setData(title, description, status);
}

void CPlaylistDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // convering editor
    const auto editorWidget = qobject_cast<playlistEditorWidget*>(editor);
    const auto newTitle = editorWidget->getTitle();
    const auto newDescription = editorWidget->getDescription();
    // updating data
    // title
    model->setData(index, newTitle, Qt::DisplayRole);
    // description
    model->setData(index, newDescription, Qt::ToolTipRole);
}

/*
 * EDITOR WIDET DEFINITIONS
 */

CPlaylistDelegate::playlistEditorWidget::playlistEditorWidget(const QFont& title, const QFont& desc, const QFont& status, const QStyleOptionViewItem &opt, const QMargins margin, QWidget* parent) :
    QWidget(parent),
    title_ptr(std::unique_ptr<QLineEdit>(new QLineEdit(this))),
    descr_ptr(std::unique_ptr<QLineEdit>(new QLineEdit(this))),
    titleFont(title),
    descriptionFont(desc),
    statusFont(status),
    option(opt),
    margins(margin)
{
    // creating layout
    QVBoxLayout* lay = new QVBoxLayout();
    lay->setContentsMargins(margins);
    lay->setSpacing(2);

    // setting aditional requirements for line edits
    // title line
    title_ptr->resize(150, 15);
    title_ptr->setFrame(false);
    title_ptr->setFont(titleFont);

    // description
    descr_ptr->resize(100, 15);
    descr_ptr->setFrame(false);
    title_ptr->setFont(descriptionFont);

    // setting layout
    lay->addWidget(title_ptr.get());
    lay->addWidget(descr_ptr.get());
    lay->addSpacing(15);
    this->setLayout(lay);
}

void CPlaylistDelegate::playlistEditorWidget::setData(const QString &title, const QString &description, const QString &status)
{
    title_ptr->setText(title);
    descr_ptr->setText(description);
    statusTip = status;
}

void CPlaylistDelegate::playlistEditorWidget::paintEvent(QPaintEvent *event)
{
    // painting widgets
    QPainter painter(this);
    painter.save();
    painter.fillRect(event->rect(), option.backgroundBrush);
    painter.restore();

    // fragments quantity
    painter.setFont(statusFont);
    QApplication::style()->drawItemText(&painter, event->rect().translated(3, 18+15), title_ptr->alignment(), QPalette(), true, statusTip);
}

