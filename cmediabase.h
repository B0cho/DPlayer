#ifndef CMEDIABASE_H
#define CMEDIABASE_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <memory>

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
	


private:
    // members
    std::unique_ptr<QSqlDatabase> _playlistsDb;
    const QString DBfileName =  "dziobakplayerDB.db";
    //std::shared_ptr<QList<QDir>> _directoriesPtr;
    // methods
    bool addDatabase(const QString path);
	
public slots:
    bool loadDatabase(QFileInfo database_path);
    bool createDatabase(QFileInfo database_path);
    bool clearDatabase();
	
signals:
    void DatabaseLoaded(const bool database_loaded) const;
    void DatabaseCreated(const QFileInfo database_created) const;

};

#endif // CMEDIABASE_H
