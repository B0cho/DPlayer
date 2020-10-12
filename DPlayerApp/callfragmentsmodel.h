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
/// Represents all provided fragments and their binding with provided playlists (if occures)
class CAllFragmentsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    CAllFragmentsModel(const boost::shared_ptr<const CMFragmentsQList> allFragmentsList,  const boost::shared_ptr<const CMPlaylistQList> allPlaylists);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    const CMediaPlaylist* findPlaylist(const CMediaFragment* fragment) const; /// Returns const CMediaPlaylist, in which given fragment is linked. Return nullptr if not linked.
private:
    const boost::shared_ptr<const CMFragmentsQList> _fragments;
    const boost::shared_ptr<const CMPlaylistQList> _playlists;
};

#endif // CALLFRAGMENTSMODEL_H
