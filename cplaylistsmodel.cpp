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

    // status status tip role - playlist size
    if(role == Qt::StatusTipRole)
        return QString::number(_pointer->at(index.row()).size()) + QString(" fragements");

    // FontRole	- dedicated font
    if(role == Qt::FontRole)
        return QFont("Calibri");

    // TextAlignmentRole - align left
    if(role == Qt::TextAlignmentRole)
        return Qt::AlignLeft;

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
    Q_UNUSED(parent);
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
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
    {
        // if selected is 'all' playlist
        if(index.row())
            return Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
        else
            return Qt::ItemIsDragEnabled | defaultFlags;
    }
    else
         return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions CPlaylistsModel::supportedDragActions() const
{
    return Qt::CopyAction;
}

Qt::DropActions CPlaylistsModel::supportedDropActions() const
{
    return Qt::CopyAction;
}

QStringList CPlaylistsModel::mimeTypes() const
{
    QStringList types;
    // supported mime types for cmediafragment and cmediaplaylist
    types << CInternalMime<void>::fragmentMimeType << CInternalMime<void>::playlistMimeType;
    return types;
}

QMimeData *CPlaylistsModel::mimeData(const QModelIndexList &indexes) const
{
    // preparing data
    CInternalMime<CMediaPlaylist>* data = new CInternalMime<CMediaPlaylist>(CInternalMime<void>::playlistMimeType);

    // collecting fragments from selected playlists
    foreach (auto index, indexes) {
        // appending
        const auto val = &_pointer->operator [](index.row());
        data->container.append(val);
    }
    return data;
}

bool CPlaylistsModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(action);
    Q_UNUSED(column);

    // if data and its format is correct and if row is not "all" playlist (0 row)
    if(data && (data->hasFormat(CInternalMime<void>::fragmentMimeType) || (data->hasFormat(CInternalMime<void>::playlistMimeType))) && row && parent.isValid())
        return true;
    else
        return false;
}

bool CPlaylistsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(parent);
    qDebug() << "> Dropped data on playlist list: row: " << row;

    // if not ignore, if not dropped on existing item
    if(action == Qt::IgnoreAction || (row == -1 && !parent.isValid()))
        return true;

    // dropped fragments
    qDebug() << ">> Dropped fragments";

    // getting row
    row = parent.row();

    // getting playlist
    CMediaPlaylist* playlist = &_pointer->operator [](row);

    // inserting fragments to be copied
    QList<CMediaFragment*> inserted;
    QList<CMediaFragment> toBeInserted;
    if(data->hasFormat(CInternalMime<void>::playlistMimeType))
    {
        const QList<const CMediaPlaylist*> playlistData = dynamic_cast<const CInternalMime<CMediaPlaylist>*> (data)->container;
        foreach (auto plist, playlistData) {
            std::transform(plist->getList().cbegin(),
                           plist->getList().cend(),
                           std::back_inserter(toBeInserted),
                           [](const CMediaFragment* i)->CMediaFragment{ return *i; });
        }
    }
    else if(data->hasFormat(CInternalMime<void>::fragmentMimeType))
    {
        const QList<const CMediaFragment*> playlistData = dynamic_cast<const CInternalMime<CMediaFragment>*> (data)->container;
        std::transform(playlistData.cbegin(), playlistData.cend(),
                       std::back_inserter(toBeInserted),
                       [](const CMediaFragment* i)->const CMediaFragment{ return *i; });
    }
    else
        return false;
    emit PMODEL_appendFragments(toBeInserted, inserted);

    // finding new names
    QStringList usedTitles;
    std::transform(playlist->getList()->cbegin(),
                   playlist->getList()->cend(), std::back_inserter(usedTitles),
                   [](const CMediaFragment* i)->const QString{ return i->title(); });

    // changing titles
    foreach (CMediaFragment* copied, inserted) {
        QString title = copied->title();
        title = CDatabaseMember::findNewTitle(title, usedTitles);
        usedTitles.append(title);
        copied->setTitle(title);
    }

    // adding fragments to playlist
    playlist->addFragment(inserted);

    // saving database
    emit PMODEL_saveDatabase();
    return true;
}

int CPlaylistsModel::getRow(const CMediaPlaylist *playlist) const
{
    int row = 0;
    foreach (const auto elem, *_pointer) {
        if(elem.id() == playlist->id()) return row;
        row++;
    }
    return -1;
}
