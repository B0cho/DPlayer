#include "csettings.h"

/*!
  \class CSettings
  \brief Class, which holds app settings and is responsible for its reading and saving
  \inmodule Initialization
  This class holds all the settings in its members and uses QSettings to communicate with local registry.
  Moreover, it reads data from registry or creates \l regkey if there is no one.
  Then, it creates \l SettingsWizard to complete data from user, calls \l CMediaBase to create/read databases.
  If no \l regkey was created/found, no changes will be saved (until it is changed in app settings), as with databases.
  It is also in charge of saving data to \l regkey, also when app is being closed.
  \note Before using, connect class \b {signals and slots} with proper signals and slots of \l SettingsWizard and \l CmediaBase!
  \note Class instance is not enough! Before using, use \l init()!
  \note There should be only one instance of the class!
  Class holds following data:
  \list
    \li Last application launch date.
    \li \l Regkey creation date.
    \li Last played media path.
    \li Settings initialization status.
    \li Paths to search for media.
  \endlist

  \target regkey
  \chapter Registry key
  Registry key to hold settings. It is created with name set in \c main() for QCoreApplication-
  setApplicationName, setApplicationVersion and setOrganisationName. By default it is created in \c HKEY_CURRENT_USER/Software and
  consists of following keys:
  \list
    \li created
    \li opened
    \li paths
        \list
        \li path to music album
        \li second path to directory
        \li ...
        \li path to video directory
        \endlist
  \endlist
  */

/*!
 * \brief Default constructor of the objects.
 * Sets _init to false- assumes that settings haven't been loaded.
 */

CSettings::CSettings(QObject *parent) : QObject(parent)
{
    _init = false;
}

/*!
 * \brief Default destructor.
 * By default, should be destroyed when the app is being closed. Then, saves changes with \l saveSettings(), sending \c true argument.
 */

CSettings::~CSettings()
{
	saveSettings(true);
}

/*!
 * \brief Inits all the settings of the app- reads from registry, loades databases etc.
 * This method should set at very begining, right after CSettings creation. When started, checks whether \l regkey exists in registry.
 * If yes, then calls \l readSettings(). If no, creates \l SettingsWizard, connects \l dataCompleted and \l wizardDataProcessed signals
 * with \l settingsWizardData and \l settingsWizardFeedback slots. Then, shows SettingsWizard using \b exec().
 */

void CSettings::Init()
{
    // checking whether register key exists
    if(!_reg.contains(creationdate))
    {
        // creating wizard
        SettingsWizard wiz;
        connect(&wiz, SIGNAL(dataCompleted(settingsWizardData)), this, SLOT(wizardData(settingsWizardData)));
        connect(this, SIGNAL(wizardDataProcessed(settingsWizardFeedback)), &wiz, SLOT(wizardFeedback(settingsWizardFeedback)));
        wiz.exec();
        //
    } else readSettings();
}

/*!
 * \brief Used to save settings to registry and databases.
 * \a exit - \c true, when app is going to be closed. By default \c false.
 * Saves class settings to registry to app \l regkey, if it was intended to do so.
 * If yes, saves \l _lastDate (which is updated), \l _creationDate and \l _paths.
 */

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

/*!
 * \brief Reads settings from \l regkey.
 * This method is resposible for reading data from \l regkey and saving it to CSettings members.
 * At the end emits \l loadDBs signal, in order to load \l {playlistsDatabase}{playlists} and \l {fragmentsDatabase}{fragments} databases.
 */

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

/*!
 * \brief Nothing but erasing \l regkey from local registry.
 */

void CSettings::clearRegKeys()
{
	_reg.clear();
}

/*!
 * \brief Slot, which takes \c bool parameters to inform CSettings about playlists and fragments databases loading.
 * \b {Parameters:} \i{playlists, fragments}
 */

void CSettings::DBsLoadResult(const bool playlists, const bool fragments)
{

}

/*!
 * \brief Feedback for CSettings about creating databases.
 * \b {Parameters:} \i{playlists, fragments}
 * Takes two \c QFileInfo path-arguments: playlists and fragments databases. Then, assigns paths to \l feedback, which is sent later to settingsWizard.
 * \warning If no database was created, pass \b empty QFileInfo!
 */

void CSettings::DBsCreateResult(const QFileInfo playlists, const QFileInfo fragments)
{
    feedback.fragmentsPath = playlists.absoluteFilePath();
    feedback.playlistsPath = fragments.absoluteFilePath();
}

/*!
 * \brief Slot, which settingsWizard communicates with
 * \b {Parameters:} \i{data} - collected from \l SettingsWizard
 * When this slot is triggered, passed \c data is being used to create playlists and fragments databases-
 * \l createDBs is emitted, and after that \l wizardDataProcessed to set \l feedback for \l settingsWizard.
 */

void CSettings::wizardData(settingsWizardData data)
{
    _init = true;
    _creationDate = QDateTime::currentDateTime();
    // saving to registry
    saveSettings();
    feedback.regkeyCreated = true;
    // creating database
    emit createDBs(QFileInfo(data.playlistsDirectory), QFileInfo(data.fragmentsDirectory));
    emit wizardDataProcessed(feedback);
}

/*!
  \fn CSettings::createDBs(const QFileInfo playlists, const QFileInfo fragments) const
  This signal is emitted to triger databases creation.
  */
/*!
 * \fn CSettings::loadDBs(const QFileInfo playlists, const QFileInfo fragments) const
 * This signal should be emitted when databases reading is required.
 * */
/*!
 * \fn CSettings::wizardDataProcessed(const settingsWizardFeedback) const
 * Emitted, when data from \l SettingsWizard was processed. Sends results as \l settingsWizardFeedback to \l SettingsWizard.
 * */
