#include "cfragmentsmodel.h"

CFragmentsModel::CFragmentsModel()
{
    _listPointer = nullptr;
}

QVariant CFragmentsModel::data(const QModelIndex &index, int role) const
{
    const auto i = index.row() + 1; // correction for map
    /// COPIED FROM PLAYLISTSMODEL
    // if there is smth wrong with index
    if(!index.isValid() || index.row() >= _listPointer->size())
        return QVariant();

    // edit or display role - label
    if(role == Qt::DisplayRole)
        return _listPointer->value(i)->title();

    // tooltip role - description
    if(role == Qt::ToolTipRole)
        return _listPointer->value(i)->desc();

    // if nothing from above complies
    return QVariant();
}

int CFragmentsModel::rowCount(const QModelIndex &parent) const
{
    return (_listPointer) ? _listPointer->count() : 0;
}

Qt::DropActions CFragmentsModel::supportedDropActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

Qt::DropActions CFragmentsModel::supportedDragActions() const
{
    return Qt::MoveAction | Qt::CopyAction;
}

Qt::ItemFlags CFragmentsModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags = QAbstractListModel::flags(index);
    if (index.isValid())
         return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
    else
         return Qt::ItemIsDropEnabled | defaultFlags;
}

QStringList CFragmentsModel::mimeTypes() const
{
    QStringList types;
    // support for mime types
    types << CInternalMime<void>::fragmentMimeType;
    return types;
}

QMimeData *CFragmentsModel::mimeData(const QModelIndexList &indexes) const
{
    // preparing data for mime
    CInternalMime<CMediaFragment>* data = new CInternalMime<CMediaFragment>(CInternalMime<void>::fragmentMimeType);

    foreach (auto index, indexes) {
        if(index.isValid())
        {
            // for each index find key for given fragment of model and append it
            const auto key = index.row() + 1;
            const auto val = _listPointer->value(key);
            data->container.append(val);
        }
    }
    return data;
}

bool CFragmentsModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    qDebug() << "> Drop on " << this;
    Q_UNUSED(action);
    Q_UNUSED(row);
    Q_UNUSED(parent);
    Q_UNUSED(column);
    // if data and format is correct
    if(data && data->hasFormat(CInternalMime<void>::fragmentMimeType)) return true;
    else return false;
}

bool CFragmentsModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    Q_UNUSED(column);
    Q_UNUSED(parent);
    qDebug() << "> Dropped data on fragments list: row: " << row;

    // if not ignore
    if(action == Qt::IgnoreAction)
        return true;

    // if dropped Fragment
    if(data->hasFormat(CInternalMime<void>::fragmentMimeType))
    {
        qDebug() << ">> Dropped fragments";

        // casting to fragments
        const QList<const CMediaFragment*> fragmentsData = dynamic_cast<const CInternalMime<CMediaFragment>*>(data)->container;

        // checking if to move or to copy
        if(row == -1 && parent.isValid())
        {
            // move
            qDebug() << ">>> Moving";
            row = parent.row() + 1;

            CMFragmentsMap newMap;
            // inserting and removing dropped items at their place
            foreach (const auto i, fragmentsData) {
                qDebug() << ">>>> Moving id: " << i->id() << " title: " << i->title() << " to row: " << row;
                newMap.insert(row++, i);
                _listPointer->remove(_listPointer->key(i));
            }

            // inserting lasting data from old map
            row = 1;
            foreach (const auto i, *_listPointer) {
                while (newMap.value(row)) {
                    row++;
                }
                newMap.insert(row++, i);
            }

            // swapping maps
            _listPointer->swap(newMap);
            /// TO DO
            ///
            return true;
        }
        else
        {
            // copy
            qDebug() << ">>> Copying";
            // getting row for qmap
            row = _listPointer->lastKey() + 1;

            // inserting copies to mediabase fragments list
            QList<CMediaFragment*> inserted;
            QList<CMediaFragment> toBeInserted;
            std::transform(fragmentsData.cbegin(),
                           fragmentsData.cend(), std::back_inserter(toBeInserted),
                           [](const CMediaFragment* i)->const CMediaFragment{ return *i; });
            emit FMODEL_appendFragments(toBeInserted, inserted);

            // finding new names
            // getting used titles in scope of model
            QStringList usedTitles;
            std::transform(_listPointer->cbegin(),
                           _listPointer->cend(), std::back_inserter(usedTitles),
                           [](const CMediaFragment* i)->const QString{ return i->title(); });
            // changing title
            foreach (CMediaFragment* newOne, inserted) {
                QString title = newOne->title();
                title = CDatabaseMember::findNewTitle(title, usedTitles);
                usedTitles.append(title);
                newOne->setTitle(title);
            }

            // inserting to model
            beginInsertRows(QModelIndex(), row, row + inserted.count() - 1);
            foreach (const auto insert, inserted) {
                _listPointer->insert(row++, insert);
                qDebug() << ">>> appended - id: " << insert->id() << " title: " << insert->title();
            }
            endInsertRows();
            return true;
        }
    }
    return false;
}

void CFragmentsModel::FMODEL_setListPointer(CMFragmentsMap *listPointer)
{
    // send signal about change in model
    beginResetModel();
    //
    _listPointer = listPointer;
    //
    endResetModel();
}
