#include "cfragmentdelegate.h"

constexpr QSize CFragmentDelegate::itemSize;
constexpr QMargins CFragmentDelegate::itemMargins;

CFragmentDelegate::CFragmentDelegate(QWidget *parent)
{
    Q_UNUSED(parent);
}

void CFragmentDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(!index.isValid()) return;

    // obtaining data
    const auto& title = index.data(Qt::DisplayRole).toString();
    const auto& file = index.data(Qt::ToolTipRole).toString();
    const auto& scope = index.data(Qt::StatusTipRole).toString();
    const auto& font = index.data(Qt::FontRole).value<QFont>();
    qDebug() << "Painting: " << title;

    // drawing
    // background
    painter->fillRect(option.rect, (option.state & QStyle::State_Selected) ? option.palette.highlight() : option.palette.background());
    auto rect = option.rect.marginsRemoved(itemMargins);

    // original file
    painter->setFont(font);
    QApplication::style()->drawItemText(painter, rect, Qt::AlignVCenter, option.palette, true, file);

    // scope
    QApplication::style()->drawItemText(painter, rect, Qt::AlignBottom, option.palette, true, scope);

    // title
    QFont titleFont{font};
    titleFont.setBold(true);
    painter->setFont(titleFont);
    QApplication::style()->drawItemText(painter, rect, Qt::AlignTop, option.palette, true, title);
}

QSize CFragmentDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    return itemSize;
}

QWidget *CFragmentDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    QWidget* widget = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout();
    QLineEdit* titleEdit = new QLineEdit();
    auto font = index.data(Qt::FontRole).value<QFont>();

    // setting title edit
    font.setBold(true);
    titleEdit->setFont(font);
    titleEdit->setFixedHeight(15);
    titleEdit->setFrame(false);

    // bulding layout
    layout->setContentsMargins(itemMargins);
    layout->addWidget(titleEdit);
    layout->addSpacing(80);

    // setting layout and returning widget
    widget->setLayout(layout);
    return widget;
}

void CFragmentDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit* titleEdit = qobject_cast<QLineEdit*>(editor->layout()->itemAt(0)->widget());
    titleEdit->setText(index.data(Qt::DisplayRole).toString());
}

void CFragmentDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const auto titleEdit = qobject_cast<QLineEdit*>(editor->layout()->itemAt(0)->widget());
    model->setData(index, titleEdit->text());
}
