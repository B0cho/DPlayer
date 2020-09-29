#include "csearchwidget.h"

CSearchWidget::CSearchWidget(QWidget *parent):
    QWidget(parent),
    layout(new QVBoxLayout(this)), // layout init
    lineEdit(new QLineEdit()), // edit init
    suggestionView(new QListView()) // list init
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

    // connections
    connect(lineEdit.get(), SIGNAL(textChanged(const QString&)), this, SLOT(textChanged(const QString&))); // when line edit has changed
}

void CSearchWidget::setModel(QAbstractItemModel *model)
{
    suggestionView->setModel(model);
}

void CSearchWidget::textChanged(const QString &text)
{
    if(text.size())
    {
        suggestionView->show();
    }
    else
        suggestionView->hide();
}
