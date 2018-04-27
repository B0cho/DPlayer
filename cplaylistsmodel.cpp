#include "cplaylistsmodel.h"

CPlaylistsModel::CPlaylistsModel(const boost::shared_ptr<CMPlaylistQList> playlistsPointer, QObject* parent) : _pointer(playlistsPointer), QAbstractListModel(parent)
{

}

QVariant CPlaylistsModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        // label
        if(!index.isValid() || index.row()) return QVariant();
        else return _pointer->at(index.row()).title;
        break;
    case Qt::ToolTipRole:
        // hint
        //return _pointer->at(index.row()).description;
        return QString("AAA");
        break;
    default:
        return QVariant();
        break;
    }
}

int CPlaylistsModel::rowCount(const QModelIndex &parent) const
{
    return _pointer->count();
}



