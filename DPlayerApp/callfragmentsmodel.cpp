#include "callfragmentsmodel.h"

CAllFragmentsModel::CAllFragmentsModel(const boost::shared_ptr<CMFragmentsQList> allFragmentsList):
    _fragments(allFragmentsList)
{
    //
}

QVariant CAllFragmentsModel::data(const QModelIndex &index, int role) const
{
    const auto i = index.row();
    // if there is smth wrong with index
    if(!index.isValid() || index.row() >= _fragments->size())
        return QVariant();

    // edit or display role - label
    if(role == Qt::DisplayRole)
        return _fragments->at(i).title();

    return QVariant();
}

int CAllFragmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _fragments->size();
}
