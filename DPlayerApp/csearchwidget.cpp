#include "csearchwidget.h"

CSearchWidget::CSearchWidget(QWidget *parent):
    QWidget(parent),
    layout(new QVBoxLayout(this)), // layout init
    lineEdit(new QLineEdit()), // edit init
    suggestionView(new QListView()) // list init
{
    // adding widgets to layout
    layout->addWidget(lineEdit.get());
    layout->addWidget(suggestionView.get());


}
