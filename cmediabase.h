#ifndef CMEDIABASE_H
#define CMEDIABASE_H

#include <QObject>
#include <QDir>
#include <QFileInfo>

class CMediaBase
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
	const QString _baseFileName = "dplayerDB";
	QFileInfo _dbPath;
	bool _loaded;
	
public slots:
	bool loadDatabase(const QFileInfo path);
	bool createDatabase(const QDir dir);
	
signals:
	void DatabaseLoaded(const bool result) const;
	void DatabaseCreated(const bool result, const QFileInfo localisation) const;

}

#endif // CMEDIABASE_H