#ifndef CFRAGMENTSMODEL_H
#define CFRAGMENTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "cmediaplaylist.h"

/// FRAGMENTS MODEL
typedef QMap<int, const CMediaFragment*> CMFragmentsMap;
class CFragmentsModel : public QAbstractListModel
{
public:
    /// model things
    CFragmentsModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;

public slots:
    void setListPointer(CMFragmentsMap *listPointer); // sets list pointer

private:
    CMFragmentsMap* _listPointer; // keeps pointer to fragment map of current playlist

};

#endif // CFRAGMENTSMODEL_H
