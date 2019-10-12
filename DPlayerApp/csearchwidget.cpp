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
}
