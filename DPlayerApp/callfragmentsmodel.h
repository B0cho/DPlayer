#ifndef CALLFRAGMENTSMODEL_H
#define CALLFRAGMENTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <boost/shared_ptr.hpp>
#include <QList>
#include "cmediafragment.h"

typedef QList<CMediaFragment> CMFragmentsQList;
class CAllFragmentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CAllFragmentsModel(const boost::shared_ptr<CMFragmentsQList> allFragmentsList);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
private:
    const boost::shared_ptr<CMFragmentsQList> _fragments; // keeps pointer to all fragments in order to modify fragments
};

#endif // CALLFRAGMENTSMODEL_H
