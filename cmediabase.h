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
    const bool isLoaded() const;
	// members
	
private:
	// members
    QFileInfo _databaseFile;
    QSqlDatabase _playlistsDb;
	
public slots:
    bool loadDatabase(const QFileInfo database_path);
    bool createDatabase(const QFileInfo database_path);
	
signals:
    void DatabaseLoaded(const bool database_loaded) const;
    void DatabaseCreated(const QFileInfo database_created) const;

};

#endif // CMEDIABASE_H
