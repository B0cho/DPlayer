#include "csettings.h"

CSettings::CSettings(QObject *parent) : QObject(parent)
{
    _init = true;
}

CSettings::~CSettings()
{
	saveSettings(true);
}

void CSettings::Init()
{
    // checking whether register key exists
    if(!_reg.contains(creationdate))
	{
		// creating wizard
		SettingsWizard wiz;
		if(wiz.exec())
		{
			// reading user input

            _creationDate = _lastDate = QDateTime::currentDateTime();
			// READING PATHS
			
            // saving data to registry
            //saveSettings();
            // creating dbs
            //emit createDBs(QFileInfo("wtf", playlists), QFileInfo("wtf", fragments));
        } else _init = false;
    } else readSettings();
}


void CSettings::saveSettings(const bool exit)
{
	// if initialised
	if(!_init) return;
	// if app ends
    if(exit) _lastDate = QDateTime::currentDateTime();
	// dates and dbase
    _reg.setValue(creationdate, _creationDate);
    _reg.setValue(lastdate, _lastDate);

	// paths
	_reg.beginWriteArray(paths);
	for(int i = 0; i < _paths.size(); i++)
	{
		_reg.setArrayIndex(i);
        _reg.setValue(paths, _paths.at(i).absolutePath());
	}
	_reg.endArray();
}

void CSettings::readSettings()
{
	if(!_init) return;
	// dates and db
    _creationDate = _reg.value(creationdate).toDateTime();
    _lastDate = _reg.value(lastdate).toDateTime();
	// paths
	_paths.clear();
	const int size = _reg.beginReadArray(paths);
    for(int i = 0; i < size; i++) _paths.append(_reg.value(paths).toString());
    // loading db
    emit loadDBs(QFileInfo(_reg.value(playlists).toString()), QFileInfo(_reg.value(fragments).toString()));
}

void CSettings::clearRegKeys()
{
	_reg.clear();
}

void CSettings::DBsLoadResult(const bool playlists, const bool fragments)
{

}

void CSettings::DBsCreateResult(const QFileInfo playlists, const QFileInfo fragments)
{

}
