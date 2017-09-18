#ifndef CMEDIABASE_H
#define CMEDIABASE_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>

class CMediaBase: public QObject
{
	Q_OBJECT
public:
	// ctor dtor
	explicit CMediaBase(QObject *parent = nullptr);
	~CMediaBase();
	
	// methods
    const bool isPlaylists() const;
    const bool isFragments() const;
	// members
	
private:
	// members
    QFileInfo _playlistsFile, _fragmentsFile;
    QSqlDatabase _playlistsDb, _fragmentsDb;
	
public slots:
    bool loadDatabases(const QFileInfo playlists, const QFileInfo fragments);
    bool createDatabases(const QFileInfo playlists, const QFileInfo fragments);
	
signals:
    void DatabasesLoaded(const bool playlists, const bool fragments) const;
    void DatabasesCreated(const QFileInfo playlists, const QFileInfo fragments) const;

};

#endif // CMEDIABASE_H
