#ifndef CFRAGMENTSMODEL_H
#define CFRAGMENTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "cmediaplaylist.h"
#include "cfragmentmime.h"

/// FRAGMENTS MODEL
typedef QMap<int, const CMediaFragment*> CMFragmentsMap;
class CFragmentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /// model things
    CFragmentsModel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

public slots:
    void FMODEL_setListPointer(CMFragmentsMap *listPointer); // sets list pointer

signals:
    void FMODEL_appendFragments(QList<CMediaFragment> toBeCopied, QList<CMediaFragment*>& inserted); // demands mediabase to append toBeCopied fragments into its global fragments list and return pointers to them

private:
    CMFragmentsMap* _listPointer; // keeps pointer to fragment map of current playlist

};

#endif // CFRAGMENTSMODEL_H
