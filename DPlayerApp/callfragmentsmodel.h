#ifndef CALLFRAGMENTSMODEL_H
#define CALLFRAGMENTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <boost/shared_ptr.hpp>
#include <QList>
#include <algorithm>
#include "cmediafragment.h"
#include "cmediaplaylist.h"

typedef QList<CMediaFragment> CMFragmentsQList;
typedef QList<CMediaPlaylist> CMPlaylistQList;
class CAllFragmentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CAllFragmentsModel(const boost::shared_ptr<const CMFragmentsQList> allFragmentsList,  const boost::shared_ptr<const CMPlaylistQList> allPlaylists);
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    const CMediaPlaylist* findPlaylist(const CMediaFragment* fragment) const;
private:
    const boost::shared_ptr<const CMFragmentsQList> _fragments; // keeps pointer to all fragments in order to modify fragments
    const boost::shared_ptr<const CMPlaylistQList> _playlists;
};

#endif // CALLFRAGMENTSMODEL_H
