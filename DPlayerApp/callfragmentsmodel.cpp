#include "callfragmentsmodel.h"

CAllFragmentsModel::CAllFragmentsModel(const boost::shared_ptr<const CMFragmentsQList> allFragmentsList, const boost::shared_ptr<const CMPlaylistQList> allPlaylists):
    _fragments(allFragmentsList),
    _playlists(allPlaylists)
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

    // description - Playlist: + original file
    if(role == Qt::ToolTipRole)
        return "Playlist: " + findPlaylist(&_fragments->at(index.row()))->title;

    // status tip role - scope of fragment
    if(role == Qt::StatusTipRole)
    {
        const QTime beg = _fragments->at(i).getStart();
        const QTime end = _fragments->at(i).getEnd();
        return _fragments->at(i).desc() + " " + beg.toString("hh:mm:ss") + QString(" - ") + end.toString("hh:mm:ss");
    }

    return QVariant();
}

int CAllFragmentsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _fragments->size();
}

const CMediaPlaylist *CAllFragmentsModel::findPlaylist(const CMediaFragment *fragment) const
{
    const auto playlist = std::find_if(_playlists->cbegin(), _playlists->cend(), [=](const CMediaPlaylist i)->bool{
        return i.getPosition(fragment) != 0; });
    return &*playlist;
}
