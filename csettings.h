#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QDir>
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
    QFileInfo _lastpath;
    QDateTime _lastDate;
    QDateTime _creationDate;
    QList<QDir> _paths;
	bool _init;
    settingsWizardFeedback feedback;

    // methods
	
	// keys names
	const QString
	database = "database",
	lastdate = "opened",
	creationdate = "created",
    paths = "paths",
    playlists = "playlistsDB",
    fragments = "fragmentsDB";
	
signals:
    void wizardDataProcessed(const settingsWizardFeedback) const;
    void createDBs(const QFileInfo playlists, const QFileInfo fragments) const;
    void loadDBs(const QFileInfo playlists, const QFileInfo fragments) const;

private slots:
    void wizardData(settingsWizardData data);
    void DBsLoadResult(const bool playlists, const bool fragments);
    void DBsCreateResult(const QFileInfo playlists, const QFileInfo fragments);

public slots:    
	void saveSettings(const bool exit = false);
	void readSettings();
	void clearRegKeys();
};

#endif // CSETTINGS_H
