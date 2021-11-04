#include "csearchwidget.h"

CSearchWidget::CSearchWidget(QWidget *parent):
    QWidget(parent),
    layout(new QVBoxLayout(this)), // layout init
    lineEdit(new QLineEdit(this)), // edit init
    suggestionView(new QListView(this)), // list init
    proxyModel(this)
{
    // setting layout size policy
    auto sp_retain = suggestionView->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    suggestionView->setSizePolicy(sp_retain);
    layout->setMargin(0);

    // adding widgets to layout
    layout->addWidget(lineEdit.get());
    layout->addWidget(suggestionView.get());

    // setting widgets
    suggestionView->hide();
    lineEdit->setPlaceholderText("Search for fragments or playlists...");
    suggestionView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // connections
    connect(lineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&))); // when line edit has changed
}

void CSearchWidget::setModel(QAbstractItemModel *model)
{
    proxyModel.setSourceModel(model);
    suggestionView->setModel(&proxyModel);
}

void CSearchWidget::setItemDelegate(QAbstractItemDelegate *delegate)
{
    suggestionView->setItemDelegate(delegate);
}

void CSearchWidget::textChanged(const QString &text)
{
    if(text.size())
    {
        suggestionView->show();
        proxyModel.setFilterRegExp(QRegExp(text));
        proxyModel.sort(0);
    }
    else
        suggestionView->hide();
}
