#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QMessageBox>
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
    QFileInfo _databasePath;
	bool _init;
    settingsWizardFeedback feedback;

    // methods
	
	// keys names
	const QString
	database = "database",
	lastdate = "opened",
	creationdate = "created",
    paths = "paths";
	
signals:
    void wizardDataProcessed(const settingsWizardFeedback) const;
    void createDB(const QFileInfo database) const;
    void loadDB(const QFileInfo database) const;

private slots:
    void wizardData(settingsWizardData data);
    void DBLoadResult(const bool database_loaded);
    void DBCreateResult(const QFileInfo database_path);

public slots:    
	void saveSettings(const bool exit = false);
	void readSettings();
	void clearRegKeys();
};

#endif // CSETTINGS_H
