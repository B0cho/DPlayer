#include "creadonlycombinedmodel.h"

CReadOnlyCombinedModel::CReadOnlyCombinedModel(const std::initializer_list<boost::shared_ptr<QAbstractListModel> > &models, QObject *parent):
    QAbstractListModel(parent),
    _models(models)
{
    //
}


QVariant CReadOnlyCombinedModel::data(const QModelIndex &index, int role) const
{
    return childModelIndex(index).data(role);
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
    if(!index.isValid())
        return Qt::NoItemFlags;
    return childModelIndex(index).flags();
}
/*
QModelIndex CReadOnlyCombinedModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(column != 0 || row > rowCount())
        return QModelIndex();

    int count = 0;
    foreach(auto i, _models)
    {
        if(row < count + i->rowCount())
            return i->index(row - count);
        count += i->rowCount();
    }
    return QModelIndex();
}
*/

QModelIndex CReadOnlyCombinedModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int CReadOnlyCombinedModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QModelIndex CReadOnlyCombinedModel::childModelIndex(const QModelIndex &baseModelIndex) const
{
    if(!baseModelIndex.isValid())
       return QModelIndex();

    int count = 0;
    const int row = baseModelIndex.row();

    foreach(auto i, _models)
    {
        const int currentCount = i->rowCount();
        if (row >= count && row < count + currentCount)
           return i->index(row - count, 0);
        count += currentCount;
    }
    return QModelIndex();
}

