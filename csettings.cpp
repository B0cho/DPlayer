#include "csettings.h"

CSettings::CSettings(QObject *parent) : QObject(parent)
{
	_init = false;
}

CSettings::~CSettings()
{
	saveSettings(true);
}

void CSettings::Init()
{
	// checking whether register key exists
	if(!_reg.contains("default"))
	{
		// creating wizard
		SettingsWizard wiz;
		if(wiz.exec())
		{
			// reading user input
			_lastDir = "";
			_DBdir = wiz.getDBUrl();
			_creationDate = _lastDate = QDateTime::currentDataTime();
			// READING PATHS
			
			// saving data to registry
			_init = true;
			saveSettings();
		} else return;
	}
	else 
	{
		_init = true;
		readSettings();
	}
}

void CSettings::changeDBdir(const QUrl dir)
{
	_DBdir = dir;
	saveSettings();
}

void CSettings::saveSettings(const bool exit)
{
	// if initialised
	if(!_init) return;
	// if app ends
	if(exit) _lastDate = QDateTime::currentDataTime();
	// dates and dbase
	_reg.setValue(created, _creationDate);
	_reg.setValue(last, _lastDate);
	_reg.setValue(database, _DBdir);
	// paths
	_reg.beginWriteArray(paths);
	for(int i = 0; i < _paths.size(); i++)
	{
		_reg.setArrayIndex(i);
		_reg.setValue("path", _paths.at(i));
	}
	_reg.endArray();
}

void CSettings::readSettings()
{
	if(!_init) return;
	// dates and db
	_creationDate = _reg.value(created).toDateTime();
	_lastDate = _reg.value(opened).toDateTime();
	_DBdir = _reg.value(database).toUrl();
	// paths
	_paths.clear();
	const int size = _reg.beginReadArray(paths);
	for(int i = 0; i < size; i++) _paths.append(_reg.value("path").toUrl());
}

void CSettings::setLast()
{
	
	
}

void CSettings::clearRegKeys()
{
	_reg.clear();
}
