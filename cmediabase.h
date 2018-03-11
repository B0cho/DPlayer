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
#include <cmediafile.h>
#include <cmediafragment.h>
#include "cmediaplaylist.h"

class CMediaBase: public QObject
{
	Q_OBJECT
public:
	// ctor dtor
	explicit CMediaBase(QObject *parent = nullptr);
	~CMediaBase();
	
	// methods
    bool isLoaded() const; // checks if database is correctly loaded
    CMediaFile* newMediaFile(const QFileInfo file_info) const; // returns pointer to object of new media file, which is created basing on path. If file was not found, return 0

	// members
	


    QFileInfoList getFilesList(const QFlags<QDir::Filter> filters = QDir::Readable | QDir::Files) const;
    // returns list of files in directories basing on directories and extensions
private:
    // members
    std::unique_ptr<QList<CMediaFile>> _files;
    std::unique_ptr<QList<CMediaFragment>> _fragments;
    std::unique_ptr<QList<CMediaPlaylist>> _playlists;
    std::unique_ptr<QSqlDatabase> _database;
    const QList<QDir>* _directoriesPtr;
    const QStringList* _extensions;
    const QString DBfileName =  "dziobakplayerDB.db";

    // methods
    bool addDatabase(const QString path); // adds and opens db
    bool validateDatabaseFile() const; // checks whether db contains essential information and tables
    bool loadData(); // loads data from database: consists loading methods
    bool saveData(); // saves data to database
    bool asimilation(CMediaFile* file, const QFileInfoList& dirs) const; // tries to find file compliant to FileInfo
    bool clearDatabase(); // clears all content of database file and creates again all necessary tables
    // loading: method names explain everything
    void loadFiles(QSqlQuery* query);
    bool loadFragments(QSqlQuery* query);
    bool loadPlaylists(QSqlQuery* query);
public slots:
    bool BASE_loadDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions); // checks and opens database file
    bool BASE_createDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions); // creates database file
    bool BASE_reload(bool save = false); // reload database- saves it if necessary and loads all content again
	
signals:
    void BASE_DatabaseLoaded(const bool database_loaded) const; // result of loading database
    void BASE_DatabaseCreated(const QFileInfo database_created) const; // result of database file creating
    void BASE_DatabaseReloaded(const bool result) const; // result of reloading database file
    void BASE_DatabaseSaved(const bool result) const; // result of database saving to file

};

#endif // CMEDIABASE_H
