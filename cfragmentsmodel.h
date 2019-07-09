#ifndef CFRAGMENTSMODEL_H
#define CFRAGMENTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <boost/shared_ptr.hpp>
#include "cmediaplaylist.h"
#include "cinternalmime.h"

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
    Qt::ItemFlags flags(const QModelIndex &index) const;

    // drag and drop
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    Qt::DropActions supportedDropActions() const;
    Qt::DropActions supportedDragActions() const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    /// methods
    int getRow(const CMediaFragment* fragment) const; // returns row of provided fragment - if not found returns -1

public slots:
    void FMODEL_setListPointer(CMFragmentsMap *listPointer, const int playlistId); // sets list pointer

signals:
    void FMODEL_appendFragments(QList<CMediaFragment> toBeCopied, QList<CMediaFragment*>& inserted); // demands mediabase to append toBeCopied fragments into its global fragments list and return pointers to them
    void FMODEL_saveDatabase(); // sends signal to save base to database
    void FMODEL_isFragmentEditable(const int& playlist_id, bool& flag) const; // demands information if fragment from provided playlist can be deleted

private:
    CMFragmentsMap* _listPointer; // keeps pointer to fragment map of current playlist
    int playlistID; // ID of current playlist

};

#endif // CFRAGMENTSMODEL_H
