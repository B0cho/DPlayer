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
    bool isLoaded() const;

	// members
	


    QFileInfoList getFilesList(const QFlags<QDir::Filter> filters = QDir::Readable | QDir::Files) const;
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
    bool addDatabase(const QString path);
    bool validateDb() const;
    bool loadData();    
    bool saveData();
    bool asimilation(CMediaFile* file, const QFileInfoList& dirs) const;
    bool clearDatabase();
    // loading
    void loadFiles(QSqlQuery* query);
    void loadFragments(QSqlQuery* query);
    void loadPlaylists(QSqlQuery* query);
public slots:
    bool BASE_loadDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions);
    bool BASE_createDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions);
    bool BASE_reload(bool save = false);
	
signals:
    void BASE_DatabaseLoaded(const bool database_loaded) const;
    void BASE_DatabaseCreated(const QFileInfo database_created) const;
    void BASE_DatabaseReloaded(const bool result) const;
    void BASE_DatabaseSaved(const bool result) const;

};

#endif // CMEDIABASE_H
