#ifndef CPLAYLISTSMODEL_H
#define CPLAYLISTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QRandomGenerator>
#include <boost/smart_ptr.hpp>
#include "cmediaplaylist.h"

/// PLAYLIST MODEL
/// typdefs
typedef QList<CMediaPlaylist> CMPlaylistQList;
class CPlaylistsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    /// model things
    CPlaylistsModel(const boost::shared_ptr<CMPlaylistQList> playlistsPointer, QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    /// members
    const boost::shared_ptr<CMPlaylistQList> _pointer; // keeps ptr to playlists list

    /// methods

};

#endif // CPLAYLISTSMODEL_H
