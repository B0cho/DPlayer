#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include <QVariant>
#include <QList>
#include <QDebug>
#include <memory>
#include "Wizards/settingswizard.h"

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
    std::unique_ptr<QSettings> _reg;
    QFileInfo _lastpath;
    QDateTime _lastDate;
    QDateTime _creationDate;
    QList<QDir> _paths;
    QStringList _extens = {"*.mp3", "*.mp4"}; // default extensions
    QFileInfo _databasePath;
    settingsWizardFeedback feedback;

    // methods
	
	// keys names
	const QString
	database = "database",
	lastdate = "opened",
	creationdate = "created",
    paths = "paths",
    extensions = "extensions";
	
signals:
    void SETT_wizardDataProcessed(const settingsWizardFeedback) const;
    void SETT_createDB(const QFileInfo database, const QList<QDir>* directories, const QStringList* extensions) const;
    void SETT_loadDB(const QFileInfo database, const QList<QDir>* directories, const QStringList* extensions) const;

private slots:
    void SETT_wizardData(settingsWizardData data);
    void SETT_DBLoadResult(const bool database_loaded);
    void SETT_DBCreateResult(const QFileInfo database_path);

public slots:    
    void SETT_saveRegistrySettings(const bool exit = false);
    void SETT_readRegistrySettings();
    void SETT_clearRegSettingsKeys();
};

#endif // CSETTINGS_H
