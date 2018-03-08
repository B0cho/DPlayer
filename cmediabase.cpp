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
    \endchapter

    \target fragments
    \chapter TABLE fragments
    Contains all fragments created by user.
    \note By default, every file in \l {files}{TABLE files} has one default fragment- it is representation of whole file.
    \table
        \header
            \li Atribute type
            \li Atribute name
            \li Role
        \row
            \li frag_id
            \li INT -PRIMARY KEY; UNIQUE; NOT NULL
            \li Holds id for fragment. Used as primary key.
        \row
            \li file_path
            \li TEXT - NOT NULL
            \li Holds absolute path to the file.
        \row
            \li start
            \li INT - NOT NULL
            \li Holds a moment when fragment starts as miliseconds from file begining.
        \row
            \li end
            \li INT - NOT NULL
            \li Holds a moment when fragment ends as miliseconds from file begining.
        \row
            \li created
            \li INT - NOT NULL
            \li Holds QDateTime of fragment creation as miliseconds.
        \row
            \li title
            \li TEXT - NOT NULL
            \li Fragment title.
        \row
            \li desc
            \li TEXT - NOT NULL
            \li Fragment description.
        \endtable
        \endchapter
  */
/*!
 * \brief Default constructor
 */
CMediaBase::CMediaBase(QObject *parent): QObject(parent)
{
    //
}

/*!
 * \brief Default destructor
 */
CMediaBase::~CMediaBase()
{
    if(isLoaded()) _database->close();
}

/*!
 * \brief This slot loads SQLite3 database from given file.
 * Emits \l BASE_DatabaseLoaded signal, which informs connected slots about result.
 * \b{Parameters:} database_path
 * \b{Returns:} result of database loading
 */
bool CMediaBase::BASE_loadDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions)
{
    qDebug() << "BASE_loadDatabase fired";
    // checking whether file exists
    if(!database_path.exists() || !database_path.isReadable())
    {
        qDebug() << "> Base file not found or not readable. Return.";
        emit BASE_DatabaseLoaded(false);
        return false;
    }
    // validation
    qDebug() << "> Validation";
    bool flag = addDatabase(database_path.absoluteFilePath());
    qDebug() << ">> Database adding: " << flag;
    flag = flag && validateDb();
    qDebug() << ">> Database validation: " << flag;
    // assigning dirs
    _directoriesPtr = dirs;
    _extensions = extensions;
    // reloading data from database and directories
    BASE_reload();
    // feedback
    emit BASE_DatabaseLoaded(flag);
    qDebug() << "BASE_DatabaseLoaded sent";
    return flag;
}

/*!
 * \brief Creates SQLite3 database as given path
 * \b{Parameters:} database_path - database path to be created
 * \note database_path should not contain extension .db or any other!
 * \b{Returns:} Result of operation
 */
bool CMediaBase::BASE_createDatabase(const QFileInfo database_path, const QList<QDir>* dirs, const QStringList* extensions)
{
    // adds filename to path
    auto path = QFileInfo(database_path.absoluteFilePath(), DBfileName);
    // checks whether file exists
    if(database_path.exists() && _database)
    {
        emit BASE_DatabaseCreated(QString(""));
        return false;
    }
    // creates file and opens it
    bool flag = addDatabase(path.absoluteFilePath());
    _directoriesPtr = dirs;
    _extensions = extensions;
    // sets database structure
    if(flag) clearDatabase();
    emit BASE_DatabaseCreated( (flag) ? path : QString("") );
    return flag;
}

/*!
 * \brief Checks whether \l database is loaded
 * \b {Returns:} \c true if database is loaded; otherwise \c false
 */
bool CMediaBase::isLoaded() const
{
    return _database != NULL && _database->isOpen();
}

bool CMediaBase::BASE_reload(bool save)
{
    auto flag = loadData();
    if(save) flag = flag && saveData();
    emit BASE_DatabaseReloaded(flag);
    return flag;
}

/*!
 * \brief Adds database to class internal pointer
 * \b{Parameters:} Path to open database
 * \b{Returns:} \c true if \l database was added with success; otheriwse \c false
 */
bool CMediaBase::addDatabase(const QString path)
{
    if(_database && _database->isOpen()) return true;
    _database = std::unique_ptr<QSqlDatabase>(new QSqlDatabase);
    *_database = QSqlDatabase::addDatabase("QSQLITE");
    _database->setHostName("host");
    _database->setDatabaseName(path);
    _database->setUserName("user");
    _database->setPassword("pass");
    return _database->open();
}

/*!
 * \brief CMediaBase::validateDb
 * Checks if declared database has required three tables to store media data.
 * \return \c True, if validation of Database was succesful. \c False is returned otherwise.
 */
bool CMediaBase::validateDb() const
{
    if(!isLoaded()) return false;
    QSqlQuery query;
    QStringList res, names({"files", "fragments", "playlists"});
    query.exec("SELECT name FROM sqlite_master WHERE type = 'table'");
    while(query.next()) res.append(query.value(0).toString());
    foreach (QString s, names) {
        if(!res.contains(s)) return false;
    }
    return true;
}

/*!
 * \brief CMediaBase::loadData
 * \return
 */
bool CMediaBase::loadData()
{
    if(!isLoaded()) return false;
    QSqlQuery query;
    // files
    loadFiles(&query);
    // fragments
    loadFragments(&query);
    // playlists
    loadPlaylists(&query);




    return true;
}

/*!
 * \brief CMediaBase::loadFiles
 * \return
 */
QFileInfoList CMediaBase::getFilesList(const QFlags<QDir::Filter> filters) const
{
    QFileInfoList list;
    foreach (auto i, *_directoriesPtr) {
        list.append(i.entryInfoList(*_extensions, filters));
    }
    std::unique(list.begin(), list.end());
    return list;
}

/*!
 * \brief CMediaBase::saveDb
 * \return
 */
bool CMediaBase::saveData()
{

}

/*!
 * \brief Clears database and sets its structure up
 * \b {Returns:} \c true if process was succesful; \c false it it wasn't possible
 */
bool CMediaBase::clearDatabase()
{
    // opening transaction
    if(!isLoaded() || !_database->transaction()) return false;
    // query
    QSqlQuery query;
    // droping tables
    query.exec("DROP TABLE IF EXISTS files, fragments, playlists");
    // creating table files
    query.exec("CREATE TABLE IF NOT EXISTS files ("
               "file_id INT NOT NULL UNIQUE,"
               "path TEXT NOT NULL UNIQUE,"
               "lmodified INT,"
               "fsize INT,"
               "genre TEXT,"
               "PRIMARY KEY(path))");
    // creating table fragments
    query.exec("CREATE TABLE IF NOT EXISTS fragments ("
               "frag_id INT NOT NULL UNIQUE,"
               "file INT NOT NULL,"
               "start INT NOT NULL,"
               "end INT NOT NULL,"
               "created INT NOT NULL,"
               "title TEXT NOT NULL,"
               "desc TEXT,"
               "PRIMARY KEY(frag_id),"
               "FOREIGN KEY(file) REFERENCES files(file_id))");
    // creating table playlists
    query.exec("CREATE TABLE IF NOT EXISTS playlists ("
               "playlist_id INT NOT NULL UNIQUE,"
               "title TEXT NOT NULL,"
               "descr TEXT,"
               "PRIMARY KEY(playlist_id))");
    // executing
    return _database->commit();
}

void CMediaBase::loadFiles(QSqlQuery *query)
{
    qDebug() << "Loading FILES";
    // loading strings of files in user's directories
    auto y = _directoriesPtr->constFirst();
    auto dirs = getFilesList();
    query->exec("SELECT * FROM files");
    while(query->next())
    {
        bool flag = true;
        CMediaFile* file;
        try
        {
            QDateTime created;
            created.setMSecsSinceEpoch(query->value(3).toInt());
            file = new CMediaFile(query->value(0).toInt(),
                            query->value(1).toString(),
                            query->value(2).toInt(),
                            created,
                            query->value(4).toString());
        }
        catch(EMediaFileError e)
        {
            if(e == INVALID) flag = asimilation(file, dirs);
                else continue;
            // in case of adding signals about problems
        }
        if(flag && std::any_of(_directoriesPtr->cbegin(), _directoriesPtr->cend(),
                               [&file](const QDir d)->auto{ return  file->file().absoluteFilePath().contains(d.absolutePath()); }))
            _files->append(*file);
        delete file;
    }
    qDebug() << "FILES loaded";
}

void CMediaBase::loadFragments(QSqlQuery *query)
{
    // fragments
    qDebug() << "Loading FRAGMENTS";
    _fragments = std::unique_ptr<QList<CMediaFragment>>(new QList<CMediaFragment>);
    query->exec("SELECT * FROM fragments");
    while(query->next())
    {
        // getting file id
        int foreign_FileId = query->value(1).toInt();
        // getting pointer to file
        auto fl = std::find_if(_files->cbegin(), _files->cend(),
                [&foreign_FileId](const CMediaFile f)->auto{ return foreign_FileId == f.id(); });
        // if pointer was found
        if(fl != _files->constEnd())
        {
            QDateTime created;
            created.setMSecsSinceEpoch(query->value(4).toInt());
            _fragments->append(CMediaFragment(query->value(0).toInt(),
                               &*fl,
                               QTime(0, 0, 0, query->value(2).toInt()),
                               QTime(0, 0, 0, query->value(3).toInt()),
                               created,
                               query->value(5).toString(),
                               query->value(6).toString()));
        }
    }
    qDebug() << "FRAGMENTS loaded";
}

void CMediaBase::loadPlaylists(QSqlQuery *query)
{
    // playlists
    query->exec("SELECT * FROM playlists");
    QList<int> ids;
    while(query->next()) ids.append(query->value(0).toInt());
    foreach (auto i, ids) {
        query->prepare("SELECT * FROM ?");
        query->addBindValue(i);
        query->exec();
    }




    qDebug() << "PLAYLISTS loaded";
}


/*!
 * \brief CMediaBase::asimilation
 */
bool CMediaBase::asimilation(CMediaFile* file, const QFileInfoList& dirs) const
{

}

