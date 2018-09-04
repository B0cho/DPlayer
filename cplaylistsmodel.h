#ifndef CPLAYLISTSMODEL_H
#define CPLAYLISTSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <boost/smart_ptr.hpp>
#include "cmediaplaylist.h"
#include "cinternalmime.h"

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
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    // drag and drop
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(const QModelIndexList &indexes) const;
    bool canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);

private:
    /// members
    const boost::shared_ptr<CMPlaylistQList> _pointer; // keeps ptr to playlists list

    /// methods

signals:
    void PMODEL_getNewId(int& newId); // demands providing new id for playlist
    void PMODEL_appendFragments(QList<CMediaFragment> toBeCopied, QList<CMediaFragment*>& inserted);
    void PMODEL_saveDatabase();

public slots:
    void PMODEL_isDeleteAccepted(const QMimeData* data, bool& flag) const; // checks if provided data can be dropped

};

#endif // CPLAYLISTSMODEL_H
