#include "csearchwidget.h"

CSearchWidget::CSearchWidget(QObject *parent):
    QVBoxLayout(parent),
    suggestionView(new QListView(this)),
    lineEdit(new QLineEdit(this))
{

}
