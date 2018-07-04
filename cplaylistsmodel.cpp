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
    case Qt::EditRole:
        // label
        if(!index.isValid() || index.row()) return QVariant();
        else return _pointer->at(index.row()).title;
        break;
    case Qt::ToolTipRole:
        // hint
        return _pointer->at(index.row()).description;
        break;
    default:
        return QVariant();
        break;
    }
}

// testing

int CPlaylistsModel::rowCount(const QModelIndex &parent) const
{
    return _pointer->count();
}

bool CPlaylistsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
              qDebug() << "Setting data";
              _pointer->first().title = value.toString();
              emit dataChanged(index, index, {role});
              return true;
          }
          return false;
}

Qt::ItemFlags CPlaylistsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
              return Qt::ItemIsEnabled;

          return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}



