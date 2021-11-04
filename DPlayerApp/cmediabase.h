#ifndef CMEDIABASE_H
#define CMEDIABASE_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>
#include <QScopedPointer>
#include <memory>
#include <algorithm>
#include <boost/smart_ptr.hpp>
#include <QItemSelection>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include "cmediafragment.h"
#include "cmediaplaylist.h"
#include "cplaylistsmodel.h"
#include "cfragmentsmodel.h"
#include "callfragmentsmodel.h"

/// REPRESENTS WHOLE DATA - SETTINGS, MEDIAS AND MANAGES THEM
// typedefs
//typedef QList<CMediaFragment> CMFragmentsQList;
typedef QList<CMediaPlaylist> CMPlaylistQList;
class CMediaBase: public QObject
{
	Q_OBJECT
public:
    /// ctor dtor
	explicit CMediaBase(QObject *parent = nullptr);
	~CMediaBase();
	
    /// methods
    bool isLoaded() const;// checks if database is correctly loaded
    CMediaFragment* newMediaFile(const QFileInfo file_info) const; // returns pointer to object of new media file, which is created basing on path. If file was not found, return 0
    bool saveFragment(const CMediaFragment* fragment);// updates or writes fragment to database
    bool savePlaylist(const CMediaPlaylist* playlist); // updates or writes playlist to database
    QFileInfoList getFilesList(const QFlags<QDir::Filter> filters = QDir::Readable | QDir::Files) const; // returns list of files in directories basing on directories and extensions

    /// models access
    boost::shared_ptr<CPlaylistsModel> getPlaylistsModel() const; // returns playlist model
    boost::shared_ptr<CFragmentsModel> getFragmentsModel() const; // returns fragments model
    boost::shared_ptr<CAllFragmentsModel> getAllFragmentsModel() const; // returns model with all fragments

private:
    /// default strings - values put below the class
    static constexpr const char* allTitle = "Files";
    static constexpr const char* allDescription = "Only raw files";

    /// models
    boost::shared_ptr<CPlaylistsModel> _playlistsModel;
    boost::shared_ptr<CFragmentsModel> _fragmentsModel;
    boost::shared_ptr<CAllFragmentsModel> _allFragmentsModel;

    /// members
    boost::shared_ptr<CMFragmentsQList> _fragments; // ptr to Qlist of fragments
    boost::shared_ptr<CMPlaylistQList> _playlists; // ptr to Qlist of playlists
    std::unique_ptr<QSqlDatabase> _database; // ptr to database, that includes settings etc.
    const QList<QDir>* _directoriesPtr; // list of directories, that contain media
    const QStringList* _extensions; // list of used extensions
    const QString DBfileName =  "DplayerDB.db"; // default name of database file

    /// methods
    bool addDatabase(const QString path); // adds and opens db
    bool validateDatabaseFile() const; // checks whether db contains essential information and tables
    bool loadData(); // loads data from database: includes loading methods
    bool asimilation(CMediaFragment* file, const int& id, const int &size, const quint64 created, const QFileInfoList& dirs) const; // tries to find file compliant to FileInfo
    bool clearDatabase(); // clears all content of database file and creates again all necessary tables
    // loading: method names explain everything
    void loadFragments(QSqlQuery* query); // loads fragments from database to _fragments
    bool loadPlaylists(QSqlQuery* query); // loads playlists from database to _playlists
    static void setFragmentDuration(CMediaFragment& fragment);

public slots:
    bool BASE_loadDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions); // checks and opens database file
    bool BASE_createDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions); // creates database file
    bool BASE_reload(bool save = false); // reload database- saves it if necessary and loads all content again
    bool BASE_saveData(); // saves data to database
    void BASE_changeFragmentsList(QItemSelection selected, QItemSelection deselected); // changes list of fragments in fragments model
    void BASE_delete(const QMimeData* data); // deletes provided data
    void BASE_isDeleteAccepted(const QMimeData* data, bool& flag) const; // checks if provided data can be dropped for delete
    void BASE_isEditionAccepted(const int playlistID, bool& flag) const; // checks if deletion withing playlist of given id is acceptable
    void playSelected(const QModelIndex& selected) const;

private slots:
     void BASE_insertFragments(QList<CMediaFragment> toBeCopied, QList<CMediaFragment*>& inserted); // inserts toBeCopied to _fragments and returns pointers to them
     void BASE_newFragmentId(int& newId) const; // returns new id for new fragment (last of already stored items in fragments)
     void BASE_newPlaylistId(int& newId) const; // returns new id for new playlist (last of alerady stored items in playlists)
	
signals:
    void BASE_DatabaseLoaded(const bool database_loaded) const; // result of loading database
    void BASE_DatabaseCreated(const QFileInfo database_created) const; // result of database file creating
    void BASE_DatabaseReloaded(const bool result) const; // result of reloading database file
    void BASE_DatabaseSaved(const bool result) const; // result of database saving to file
    void playMedia(const QMediaContent&) const;

};

#endif // CMEDIABASE_H
