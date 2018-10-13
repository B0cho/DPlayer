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
#include <boost/core/noncopyable.hpp>
#include "Wizards/settingswizard.h"

class CSettings : public QObject, boost::noncopyable
{
    Q_OBJECT
public:
    // ctors dtors
    explicit CSettings(QObject *parent = nullptr);
    ~CSettings();
	// methods
    void Init();
    // const minimum size of window
    const QSize _minWindowSize = QSize(500, 500);
    // gs for window position
    QPoint windowPos() const;
    void setWindowPos(const QPoint &windowPos);

    QSize windowSize() const;
    void setWindowSize(const QSize &windowSize);

private:
    // members
    std::unique_ptr<QSettings> _reg; // ptr to regisry
    QFileInfo _lastpath; // deprecated // change to last_fragment id
    QDateTime _lastDate; // last date of usage
    QDateTime _creationDate; // data of registry creating
    QList<QDir> _paths; // media paths
    QStringList _extens = {"*.mp3", "*.mp4"}; // default extensions
    QFileInfo _databasePath; // path to database
    settingsWizardFeedback feedback; // feedback from init wizard
    // window properties
    // window size
    QSize _windowSize;
    // window position on Desktop
    QPoint _windowPos;

    // methods
	
	// keys names
	const QString
	database = "database",
	lastdate = "opened",
	creationdate = "created",
    paths = "paths",
    mainWindow = "mainWindow",
    extensions = "extensions";
	
signals:
    void SETT_wizardDataProcessed(const settingsWizardFeedback) const; //
    void SETT_createDB(const QFileInfo database, const QList<QDir>* directories, const QStringList* extensions) const; // emitted as demand to create database
    void SETT_loadDB(const QFileInfo database, const QList<QDir>* directories, const QStringList* extensions) const; // emitted as demand to load data from database
    void SETT_update() const; // emited to get all informtion and settings from other components

private slots:
    void SETT_wizardData(settingsWizardData data); // receives data from closed wizard
    void SETT_DBLoadResult(const bool database_loaded); // receives result of db loading
    void SETT_DBCreateResult(const QFileInfo database_path); // receives result of db creating

public slots:    
    void SETT_saveRegistrySettings(const bool exit = false); // saves all settings data to registry
    void SETT_readRegistrySettings(); // reads data from registry
    void SETT_clearRegSettingsKeys(); // clears registry

};

#endif // CSETTINGS_H
