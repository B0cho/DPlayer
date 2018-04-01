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
    _reg = nullptr;
}

/*!
 * \brief Default destructor.
 * By default, should be destroyed when the app is being closed. Then, saves changes with \l SETT_saveRegistrySettings(), sending \c true argument.
 */

CSettings::~CSettings()
{
    SETT_saveRegistrySettings(true);
    if(!_reg)
    {
        QSettings qs;
        qs.clear();
    }
}

/*!
 * \brief Inits all the settings of the app- reads from registry, loades databases etc.
 * This method should set at very begining, right after CSettings creation. When started, checks whether \l regkey exists in registry.
 * If yes, then calls \l SETT_readRegistrySettings(). If no, creates \l SettingsWizard, connects \l WIZZ_dataCompleted and \l SETT_wizardDataProcessed signals
 * with \l settingsWizardData and \l settingsWizardFeedback slots. Then, shows SettingsWizard using \b exec().
 */

void CSettings::Init()
{
    qDebug() << "Settings - Initialization";
    // checking whether register key exists
    QSettings qs;
    if(!qs.contains(creationdate))
    {
        // creating wizard
        qDebug() << "> Welcome Wizard";
        SettingsWizard wiz;
        connect(&wiz, SIGNAL(WIZZ_dataCompleted(settingsWizardData)), this, SLOT(SETT_wizardData(settingsWizardData)));
        connect(this, SIGNAL(SETT_wizardDataProcessed(settingsWizardFeedback)), &wiz, SLOT(WIZZ_wizardFeedback(settingsWizardFeedback)));
        qDebug() << ">> Setting connections to Wizard";
        qDebug() << ">> Launching:";
        wiz.exec();
    }
    else
    {
        // standard reading
        qDebug() << "> Standard reading settings";
        _reg = std::unique_ptr<QSettings>(new QSettings);
        SETT_readRegistrySettings();
        qDebug() << ">> SETT_readRegistrySettings sent";
    }
}

/*!
 * \brief Used to save settings to registry and databases.
 * \a exit - \c true, when app is going to be closed. By default \c false.
 * Saves class settings to registry to app \l regkey, if it was intended to do so.
 * If yes, saves \l _lastDate (which is updated), \l _creationDate and \l _paths.
 */

void CSettings::SETT_saveRegistrySettings(const bool exit)
{
    qDebug() << "SETT_saveRegistrySettings fired";
    // if initialised
    if(!_reg)
    {
        qDebug () << "> Registry saving not allowed. Return.";
        return;
    }
	// if app ends
    if(exit)
    {
        qDebug() << "> Exitting app.";
        _lastDate = QDateTime::currentDateTime();
    }
	// dates and dbase
    _reg->setValue(creationdate, _creationDate);
    _reg->setValue(lastdate, _lastDate);
    _reg->setValue(database, _databasePath.absoluteFilePath());
    qDebug() << "> Dates saved to registry.";
	// paths
    qDebug() << "> Saving paths:";
    _reg->beginWriteArray(paths);
	for(int i = 0; i < _paths.size(); i++)
	{
        _reg->setArrayIndex(i);
        auto path =  _paths.at(i).absolutePath();
        _reg->setValue(paths, path);
        qDebug() << path;
	}
    _reg->endArray();
    // extensions
    qDebug() << "> Saving extensions:";
    _reg->beginWriteArray(extensions);
    for(auto i = 0; i < _extens.size(); i++)
    {
        _reg->setArrayIndex(i);
        auto ext = _extens.at(i);
        _reg->setValue(extensions, ext);
        qDebug() << ext;
    }
    _reg->endArray();
}

/*!
 * \brief Reads settings from \l regkey.
 * This method is resposible for reading data from \l regkey and saving it to CSettings members.
 * At the end emits \l loadDBs signal, in order to load \l {playlistsDatabase}{playlists} and \l {fragmentsDatabase}{fragments} databases.
 */

void CSettings::SETT_readRegistrySettings()
{
    qDebug() << "SETT_readRegistrySettings fired";
    if(!_reg)
    {
        qDebug() << "> No registry set. Return.";
        return;
    }
    // loading dates and db
    _creationDate = _reg->value(creationdate).toDateTime();
    _lastDate = _reg->value(lastdate).toDateTime();
    _databasePath = QFileInfo(_reg->value(database).toString());
    qDebug() << "> Dates loaded from registry";
    // loading paths
    qDebug() << "> Loading paths from registry:";
	_paths.clear();
    int size = _reg->beginReadArray(paths);
    for(int i = 0; i < size; ++i)
    {
        _reg->setArrayIndex(i);
        _paths.append(QDir(_reg->value(paths).toString()));
        qDebug() << _paths.last().absolutePath();
    }
    _reg->endArray();
    // loading extensions
    qDebug() << "> Loading extensions from registry:";
    _extens.clear();
    size = _reg->beginReadArray(extensions);
    for(auto i = 0; i < size; i++)
    {
        _reg->setArrayIndex(i);
        _extens.append(_reg->value(extensions).toString());
        qDebug() << _extens.last();
    }
    _reg->endArray();
    // loading db
    emit SETT_loadDB(_databasePath, &_paths, &_extens);
    qDebug() << "SETT_loadDB sent";
}

/*!
 * \brief Nothing but erasing \l regkey from local registry.
 */

void CSettings::SETT_clearRegSettingsKeys()
{
    _reg->clear();
}

/*!
 * \brief Slot, which takes \c bool parameters to inform CSettings about playlists and fragments databases loading.
 * \b {Parameters:} \i{playlists, fragments}
 */

void CSettings::SETT_DBLoadResult(const bool database_loaded)
{
    // failed to load database notify
    if(!database_loaded)
        QMessageBox::warning(qobject_cast<QWidget*>(parent()), "Database loading failed!", "Failed to load database from " + _databasePath.absoluteFilePath() + " path. It might be deleted or moved to other loaction. You can browse it in Settings.");
}

/*!
 * \brief Feedback for CSettings about creating databases.
 * \b {Parameters:} \i{playlists, fragments}
 * Takes two \c QFileInfo path-arguments: playlists and fragments databases. Then, assigns paths to \l feedback, which is sent later to settingsWizard.
 * \warning If no database was created, pass \b empty QFileInfo!
 */

void CSettings::SETT_DBCreateResult(const QFileInfo database_path)
{
    // settings info for settings wizard
    feedback.databasePath = database_path.absoluteFilePath();
    // setting database path for regkey
    _databasePath = database_path;
    SETT_saveRegistrySettings();
}

/*!
 * \brief Slot, which settingsWizard communicates with
 * \b {Parameters:} \i{data} - collected from \l SettingsWizard
 * When this slot is triggered, passed \c data is being used to create playlists and fragments databases-
 * \l createDBs is emitted, and after that \l SETT_wizardDataProcessed to set \l feedback for \l settingsWizard.
 */

void CSettings::SETT_wizardData(settingsWizardData data)
{
    // creating registry
    if(!_reg) _reg = std::unique_ptr<QSettings>(new QSettings);
    _creationDate = QDateTime::currentDateTime();
    // saving paths from Wizard
    foreach (QString path, data.mediaDirectories) {
        _paths.append(path);
    }
    // saving to registry
    SETT_saveRegistrySettings();
    feedback.regkeyCreated = true;
    // creating database
    emit SETT_createDB(QFileInfo(data.databaseDirectory), &_paths, &_extens);
    emit SETT_wizardDataProcessed(feedback);
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
 * \fn CSettings::SETT_wizardDataProcessed(const settingsWizardFeedback) const
 * Emitted, when data from \l SettingsWizard was processed. Sends results as \l settingsWizardFeedback to \l SettingsWizard.
 * */
