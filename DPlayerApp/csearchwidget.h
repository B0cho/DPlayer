#ifndef CSEARCHWIDGET_H
#define CSEARCHWIDGET_H

#include <QObject>
#include <QWidget>
#include <QVBoxLayout>
#include <QListView>
#include <QLineEdit>
#include <memory>
#include <QSortFilterProxyModel>

/// Search widget, consists of search text edit and model list view.
/// Internally uses QSortFilterProxyModel for sorting and filtering on Qt::DisplayRole.
/// Dedicated for searching for playlists and fragments.
class CSearchWidget: public QWidget
{
    Q_OBJECT
private:
    // widgets
    const std::unique_ptr<QVBoxLayout> layout; // layout
    const std::unique_ptr<QLineEdit> lineEdit; // edit to type
    const std::unique_ptr<QListView> suggestionView; // list with suggestions

    // proxy model for search
    QSortFilterProxyModel proxyModel;
private slots:
    void textChanged(const QString& text); // launched when text in line edit changes
public:
    CSearchWidget(QWidget* parent = 0);
    void setModel(QAbstractItemModel* model); /// Set model for searching.
    void setItemDelegate(QAbstractItemDelegate* delegate); /// Set item delegate for list view.

};

#endif // CSEARCHWIDGET_H
