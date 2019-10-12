#ifndef CSEARCHWIDGET_H
#define CSEARCHWIDGET_H

/**
  SEARCH WIDGET CLASS
  */

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <memory>

class CSearchWidget: public QWidget
{
    Q_OBJECT
private:
    // widgets
    const std::unique_ptr<QVBoxLayout> layout; // layout
    const std::unique_ptr<QListView> suggestionView; // list with suggestions
    const std::unique_ptr<QLineEdit> lineEdit; // edit to type

public:
    CSearchWidget(QWidget* parent = 0);
};

#endif // CSEARCHWIDGET_H
