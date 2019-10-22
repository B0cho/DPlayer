#include "creadonlycombinedmodel.h"

CReadOnlyCombinedModel::CReadOnlyCombinedModel(const std::initializer_list<boost::shared_ptr<QAbstractListModel> > &models, QObject *parent):
    QAbstractListModel(parent),
    _models(models)
{
    //
}

QVariant CReadOnlyCombinedModel::data(const QModelIndex &index, int role) const
{

}

int CReadOnlyCombinedModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    int count = 0;
    for(auto i: _models)
        count += i->rowCount();
    return count;
}

Qt::ItemFlags CReadOnlyCombinedModel::flags(const QModelIndex &index) const
{

}

QModelIndex CReadOnlyCombinedModel::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex CReadOnlyCombinedModel::parent(const QModelIndex &child) const
{

}

int CReadOnlyCombinedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QModelIndex CReadOnlyCombinedModel::childModelIndex(const QModelIndex &baseModelIndex) const
{

}

QModelIndex CReadOnlyCombinedModel::baseModelIndex(const QModelIndex &childModelIndex) const
{

}
