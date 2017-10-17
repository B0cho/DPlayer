#include "cmediabase.h"

/*!
  \class CMediaBase
  \brief Class to hold everything connected with media database.
  This class is in charge of creating, loading and managing database of media preferences. It is also database view for rest of the application.
  \target database
  \chapter Database
  As mentioned above, a \b SQLite3 database is created. It holds on machine all information about user media preferences- fragments, playlists etc.
  Database has following tables, which are connected with foreign keys:
  \list
    \li \l files
    \li \l fragments
    \li \l playlists
   \endlist

   \target files
   \chapter TABLE files
   This table contains user files, which are contained in user favourite localisations, given in \l SettingsWizard or set in Settings.
   Table structure:
   \table
    \header
        \li Atribute type
        \li Atribute name
        \li Role
    \row
        \li rowid
    \row
        \li TEXT - PRIMARY KEY; UNIQUE; NOT NULL
        \li path
        \li Holds path to the file. It is also used as primary key.
    \row
        \li INT
        \li lmodified
        \li Holds date of last file modification.
    \row
        \li INT
        \li fsize
        \li Holds size of the file
    \row
        \li TEXT
        \li genre
        \li Genre of the file.
    \endtable
    Above information are used to identify files in the system, are also used in \l asymilation.

    \target fragments
    \chapter TABLE fragments
    Contains all fragments created by user.
    \note By default, every file in \l {files}{TABLE files} has one default fragment- it is
  */
/*!
 * \brief Default constructor
 */
CMediaBase::CMediaBase(QObject *parent): QObject(parent)
{
    _playlistsDb = NULL;
}

/*!
 * \brief Default destructor
 */
CMediaBase::~CMediaBase()
{
    if(isLoaded()) _playlistsDb->close();
}

/*!
 * \brief This slot loads SQLite3 database from given file.
 * Emits \l DatabaseLoaded signal, which informs connected slots about result.
 * \b{Parameters:} database_path
 * \b{Returns:} result of database loading
 */
bool CMediaBase::loadDatabase(QFileInfo database_path)
{
    // checking whether file exists
    if(!database_path.exists() || !database_path.isReadable())
    {
        emit DatabaseLoaded(false);
        return false;
    }
    // TODO: Add all tables presence
    bool flag = addDatabase(database_path.absoluteFilePath());
    emit DatabaseLoaded(flag);
    return flag;
}

/*!
 * \brief Creates SQLite3 database as given path
 * \b{Parameters:} database_path - database path to be created
 * \note database_path should not contain extension .db or any other!
 * \b{Returns:} Result of operation
 */
bool CMediaBase::createDatabase(QFileInfo database_path)
{
    // adds filename to path
    auto path = QFileInfo(database_path.absoluteFilePath(), DBfileName);
    // checks whether file exists
    if(database_path.exists() && _playlistsDb)
    {
        emit DatabaseCreated(QString(""));
        return false;
    }
    // creates file and opens it
    bool flag = addDatabase(path.absoluteFilePath());
    // sets database structure
    if(flag) clearDatabase();
    emit DatabaseCreated( (flag) ? path : QString("") );
    return flag;
}

/*!
 * \brief Checks whether \l database is loaded
 * \b {Returns:} \c true if database is loaded; otherwise \c false
 */
bool CMediaBase::isLoaded() const
{
    return _playlistsDb != NULL && _playlistsDb->isOpen();
}

/*!
 * \brief Adds database to class internal pointer
 * \b{Parameters:} Path to open database
 * \b{Returns:} \c true if \l database was added with success; otheriwse \c false
 */
bool CMediaBase::addDatabase(const QString path)
{
    if(_playlistsDb && _playlistsDb->isOpen()) return true;
    _playlistsDb = std::unique_ptr<QSqlDatabase>(new QSqlDatabase);
    *_playlistsDb = QSqlDatabase::addDatabase("QSQLITE");
    _playlistsDb->setHostName("host");
    _playlistsDb->setDatabaseName(path);
    _playlistsDb->setUserName("user");
    _playlistsDb->setPassword("pass");
    return _playlistsDb->open();
}

/*!
 * \brief Clears database and sets its structure up
 * \b {Returns:} \c true if process was succesful; \c false it it wasn't possible
 */
bool CMediaBase::clearDatabase()
{
    // opening transaction
    if(!isLoaded() || !_playlistsDb->transaction()) return false;
    // query
    QSqlQuery query;
    // droping tables
    bool f;
    f = query.exec("DROP TABLE IF EXISTS files");
    // creating table files
    f = query.exec("CREATE TABLE files ("
               "path TEXT NOT NULL UNIQUE,"
               "lmodified INT,"
               "fsize INT,"
               "genre TEXT,"
               "PRIMARY KEY(path))");
    // executing
    return _playlistsDb->commit();
}

