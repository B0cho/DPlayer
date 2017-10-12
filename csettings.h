#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QVariant>
#include <QList>
#include "Wizards/settingswizard.h"

/*
CHECK PROPER WORKING
*/

class CSettings : public QObject
{
    Q_OBJECT
public:
    // ctors dtors
    explicit CSettings(QObject *parent = nullptr);
    ~CSettings();
	// methods
    void Init();
	
	
private:
    // members
    QSettings _reg;
    QUrl _lastdir, _DBdir;
    QDateTime _lastDate, _creationDate;
	QList<QUrl> _paths;
	bool _init;

    // methods
	
	// keys names
	const QString
	database = "database",
	lastdate = "opened",
	creationdate = "created",
	paths = "paths";
	
signals:



public slots:
	void changeDBdir(const QUrl dir);
	void setLast();
	void saveSettings(const bool exit = false);
	void readSettings();
	void clearRegKeys();
};

#endif // CSETTINGS_H
