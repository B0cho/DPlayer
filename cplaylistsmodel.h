    #ifndef CPLAYLISTSMODEL_H
#define CPLAYLISTSMODEL_H

/// PLAYLIST MODEL
#include <QObject>
#include <QAbstractListModel>
#include <boost/smart_ptr.hpp>
#include "cmediaplaylist.h"

typedef QList<CMediaPlaylist> CMPlaylistQList;
class CPlaylistsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CPlaylistsModel(const boost::shared_ptr<CMPlaylistQList> playlistsPointer, QObject* parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    // editable - testing
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    const boost::shared_ptr<CMPlaylistQList> _pointer;
};

#endif // CPLAYLISTSMODEL_H
