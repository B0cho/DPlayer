#ifndef CSEARCHWIDGET_H
#define CSEARCHWIDGET_H

/**
  SEARCH WIDGET CLASS
  */

#include <QObject>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <memory>

class CSearchWidget: public QVBoxLayout
{
private:
    std::unique_ptr<QListView> suggestionView;
    std::unique_ptr<QLineEdit> lineEdit;

public:
    CSearchWidget(QObject* parent);
};

#endif // CSEARCHWIDGET_H
