#include "cplaylistsmodel.h"

CPlaylistsModel::CPlaylistsModel(const boost::shared_ptr<CMPlaylistQList> playlistsPointer, QObject* parent) : _pointer(playlistsPointer), QAbstractListModel(parent)
{

}

QVariant CPlaylistsModel::data(const QModelIndex &index, int role) const
{
    // if there is smth wrong with index
    if(!index.isValid() || index.row() >= _pointer->size())
        return QVariant();

    // edit or display role - label
    if(role == Qt::DisplayRole || role == Qt::EditRole)
        return _pointer->at(index.row()).title;

    // tooltip role - description
    if(role == Qt::ToolTipRole)
        return _pointer->at(index.row()).description;

    // if nothing from above complies
    return QVariant();
}

// testing

int CPlaylistsModel::rowCount(const QModelIndex &parent) const
{
    return _pointer->count();
}

bool CPlaylistsModel::insertRows(int row, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    qDebug() << "> Adding rows to playlist model - row: " << row << "count: " << count;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    // looking for unique IDs and titles
    // vars
    QList<int> newIds;
    QStringList usedTitles;
    QString newTitle = "New playlist";

    // getting already used ids and titles
    foreach (const CMediaPlaylist i, *_pointer) {   // for each playlist
        usedTitles.append(i.title); // and get title
    }

    // getting new id for each playlist
    for(int i = 0; i < count; i++) // for each new row
    {
        int newId;
        emit PMODEL_getNewId(newId);
        qDebug() << ">> New playlist id: " << newId;
        newIds.append(newId); // when not used found, append it
    }

    // appending
    foreach (auto newId, newIds) {
            newTitle = CDatabaseMember::findNewTitle("New playlist", usedTitles);
            qDebug() << ">> New playlists title: " << newTitle;
            usedTitles.append(newTitle);
            _pointer->insert(row++, CMediaPlaylist(newId, newTitle, "New playlist"));
            qDebug() << ">> appended";
    }

    endInsertRows();
    return true;
}

bool CPlaylistsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "> Removing rows from playlists model - row: " << row << " count: " << count;
    beginRemoveRows(QModelIndex(), row, row + count -1);

    // iterators for scope
    const auto beginScope = _pointer->begin() + row;
    const auto endScope = beginScope + count;

    // removing
    _pointer->erase(beginScope, endScope);

    endRemoveRows();
    return true;
}

bool CPlaylistsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
              qDebug() << "> Setting data";
              CMediaPlaylist& item = _pointer->operator [](index.row());
              item.title = value.toString();
              emit dataChanged(index, index, {role});
              return true;
          }
          return false;
}

Qt::ItemFlags CPlaylistsModel::flags(const QModelIndex &index) const
{
    // flags
    // if smth is frong
    if (!index.isValid())
              return Qt::ItemIsEnabled;

    // all except "All" playlist are editable
    if(!index.row())
        return QAbstractItemModel::flags(index);

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

Qt::DropActions CPlaylistsModel::supportedDragActions() const
{
    return Qt::CopyAction;
}
