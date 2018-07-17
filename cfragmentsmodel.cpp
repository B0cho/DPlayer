#include "cfragmentsmodel.h"

CFragmentsModel::CFragmentsModel()
{
    _listPointer = nullptr;
}

QVariant CFragmentsModel::data(const QModelIndex &index, int role) const
{
    const auto i = index.row() + 1; // correction for map
    switch (role) {
    case Qt::DisplayRole:
        // label
        if(!index.isValid() || index.row()) return QVariant();
        else return QString((*_listPointer)[i]->title());
        break;
    default:
        return QVariant();
        break;
    }
}

int CFragmentsModel::rowCount(const QModelIndex &parent) const
{
    return (_listPointer) ? _listPointer->count() : 0;
}

void CFragmentsModel::setListPointer(CMFragmentsMap *listPointer)
{
    // send signal about change in model
    beginResetModel();
    //
    _listPointer = listPointer;
    //
    endResetModel();
}
