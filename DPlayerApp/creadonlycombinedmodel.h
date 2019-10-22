#ifndef CREADONLYCOMBINEDMODEL_H
#define CREADONLYCOMBINEDMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <initializer_list>
#include <QList>
#include <boost/shared_ptr.hpp>

class CReadOnlyCombinedModel: public QAbstractListModel
{
    Q_OBJECT
private:
    QList<boost::shared_ptr<QAbstractListModel>> _models;
public:
    CReadOnlyCombinedModel(const std::initializer_list<boost::shared_ptr<QAbstractListModel>>& models, QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;


    QModelIndex childModelIndex(const QModelIndex& baseModelIndex) const;
    QModelIndex baseModelIndex(const QModelIndex& childModelIndex) const;
};

#endif // CREADONLYCOMBINEDMODEL_H
