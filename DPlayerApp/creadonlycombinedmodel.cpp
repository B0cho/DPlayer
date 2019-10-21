#include "creadonlycombinedmodel.h"

CReadOnlyCombinedModel::CReadOnlyCombinedModel(const std::initializer_list<boost::shared_ptr<QAbstractListModel> > &models):
    _models(models)
{
    //
}

QVariant CReadOnlyCombinedModel::data(const QModelIndex &index, int role) const
{

}

int CReadOnlyCombinedModel::rowCount(const QModelIndex &parent) const
{

}

int CReadOnlyCombinedModel::columnCount(const QModelIndex &parent) const
{

}

QModelIndex CReadOnlyCombinedModel::parent(const QModelIndex &child) const
{

}

Qt::ItemFlags CReadOnlyCombinedModel::flags(const QModelIndex &index) const
{

}

QModelIndex CReadOnlyCombinedModel::childModelIndex(const QModelIndex &baseModelIndex) const
{

}

QModelIndex CReadOnlyCombinedModel::baseModelIndex(const QModelIndex &childModelIndex) const
{

}
