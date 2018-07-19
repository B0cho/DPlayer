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
    // containers initialization
    _fragments = std::unique_ptr<CMFragmentsQList>(new CMFragmentsQList);
    _playlists = boost::shared_ptr<CMPlaylistQList>(new CMPlaylistQList);
    // models initialization
    _playlistsModel = boost::shared_ptr<CPlaylistsModel>(new CPlaylistsModel(_playlists));
    _fragmentsModel = boost::shared_ptr<CFragmentsModel>(new CFragmentsModel);
}

/*!
 * \brief Default destructor
 */
CMediaBase::~CMediaBase()
{
    if(isLoaded())
    {
        BASE_saveData();
        _database->close();
    }
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
        qDebug() << "> Data base file not found or not readable.";
        emit BASE_DatabaseLoaded(false);
        return false;
    }
    // validation
    qDebug() << "> Loading Database file:";
    bool flag = addDatabase(database_path.absoluteFilePath());
    qDebug() << ">> Database adding: " << flag;
    flag = flag && validateDatabaseFile();
    qDebug() << ">> Database validation: " << flag;
    // if not failed to open or validate db
    if(flag)
    {
        // assigning dirs
        _directoriesPtr = dirs;
        _extensions = extensions;
        // reloading data from database and directories
        flag = BASE_reload();
    }
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
    // load playlists and files
    QSqlQuery query;
    loadPlaylists(&query);
    loadFragments(&query);
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

CMediaFragment* CMediaBase::newMediaFile(const QFileInfo file_info) const
{
    if(!file_info.exists()) return nullptr;
    CMediaFragment* file;
    file = new CMediaFragment((!_fragments->empty()) ? _fragments->last().id() + 1 : 0, file_info);
    // setting default values
    file->setStart(CMediaFragment::startScope);
    file->setEnd(CMediaFragment::endScope);
    file->setDesc("Raw fragment");
    file->setTitle(file_info.baseName());
    return file;
}

bool CMediaBase::saveFragment(const CMediaFragment *fragment)
{
    if(!isLoaded() || !_database->transaction()) return false;
    QSqlQuery query;
    qDebug() << "> Updating fragment:";
    qDebug() << ">> id: " << fragment->id() << " path: " << fragment->file().absoluteFilePath();
    int fragId = fragment->id();
    // seeking for playlist
    auto playlist = std::find_if(_playlists->cbegin(), _playlists->cend(),
                                 [&fragment](CMediaPlaylist playlist)->bool{ return playlist.getPosition(fragment) != 0; });
    if(playlist == _playlists->cend()) return false; // when playlist was not found do not save
    // doing shit because of QT bug ;//
    QString prepare = "INSERT OR REPLACE INTO fragments (frag_id, path, created, fsize, title, start, end, desc, playlist, playlist_pos)"
                      " VALUES (:id, \":path\", :created, :fsize, \":title\", :start, :end, \":desc\", :playlist, :pos)";
    prepare.replace(QString(":id"), QString::number(fragId));
    prepare.replace(QString(":path"), fragment->file().absoluteFilePath());
    prepare.replace(QString(":created"), QString::number(fragment->file().created().toSecsSinceEpoch()));
    prepare.replace(QString(":fsize"), QString::number(fragment->file().size()));
    prepare.replace(QString(":start"), QString::number(fragment->start()));
    prepare.replace(QString(":end"), QString::number(fragment->end()));
    prepare.replace(QString(":playlist"), QString::number(playlist->id()));
    prepare.replace(QString(":pos"), QString::number(playlist->getPosition(fragment)));
    prepare.replace(QString(":title"), fragment->title());
    prepare.replace(QString(":desc"), fragment->desc());
    query.exec(prepare);
    query.finish();
    return _database->commit();
}

bool CMediaBase::savePlaylist(const CMediaPlaylist *playlist)
{
    if(!isLoaded() || !_database->transaction()) return false;
    QSqlQuery query;
    qDebug() << "> Updating playlist:";
    qDebug() << ">> id: " << playlist->id() << " path: " << playlist->title;
    // doing shit because of QT bug ;//
    QString prepare = "INSERT OR REPLACE INTO playlists (playlist_id, title, desc) VALUES (:id, \":title\", \":desc\")";
    prepare.replace(QString(":id"), QString::number(playlist->id()));
    prepare.replace(QString(":title"), playlist->title);
    prepare.replace(QString(":desc"), playlist->description);
    query.exec(prepare);
    query.finish();
    return _database->commit();
}

bool CMediaBase::BASE_reload(bool save)
{
    qDebug() << "Reloading database";
    // loading data
    auto flag = loadData();
    // saving to database if necessary
    if(save) flag = flag && BASE_saveData();
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
    qDebug() << "Adding database";
    if(_database && _database->isOpen())
    {
        qDebug() << "> Database is already opened. Return";
        return true;
    }
    qDebug() << "> Opening database...";
    _database = std::unique_ptr<QSqlDatabase>(new QSqlDatabase);
    *_database = QSqlDatabase::addDatabase("QSQLITE");
    _database->setHostName("host");
    _database->setDatabaseName(path);
    _database->setUserName("user");
    _database->setPassword("pass");
    return _database->open();
}

/*!
 * \brief CMediaBase::validateDatabaseFile
 * Checks if declared database has required three tables to store media data.
 * \return \c True, if validation of Database was succesful. \c False is returned otherwise.
 */
bool CMediaBase::validateDatabaseFile() const
{
    qDebug() << "Validating database file";
    if(!isLoaded())
    {
        qDebug() << "> Database is not loaded. Return.";
        return false;
    }
    QSqlQuery query;
    QStringList content, names({"fragments", "playlists"});
    qDebug() << "> Executing query: " <<
                query.exec("SELECT name FROM sqlite_master WHERE type = 'table'");
    while(query.next()) content.append(query.value(0).toString());
    foreach (QString s, names) {
        if(!content.contains(s))
        {
            qDebug() << "> Database has no essential table: " << s << " Return.";
            return false;
        }
    }
    qDebug() << "> Validation OK.";
    return true;
}

/*!
 * \brief CMediaBase::loadData
 * \return
 */
bool CMediaBase::loadData()
{
    qDebug() << "Loading data from database:";
    if(!isLoaded())
    {
        qDebug() << "> Database is not loaded. Return.";
        return false;
    }
    QSqlQuery query;
    loadPlaylists(&query);
    loadFragments(&query);
    return true;
}


/*!
 * \brief CMediaBase::loadFiles
 * \return
 */
QFileInfoList CMediaBase::getFilesList(const QFlags<QDir::Filter> filters) const
{
    // Preparing list of files
    QFileInfoList list;
    foreach (auto i, *_directoriesPtr) {
        list.append(i.entryInfoList(*_extensions, filters));
    }
    std::unique(list.begin(), list.end());
    return list;
}

boost::shared_ptr<CPlaylistsModel> CMediaBase::getPlaylistsModel() const
{
    return _playlistsModel;
}

boost::shared_ptr<CFragmentsModel> CMediaBase::getFragmentsModel() const
{
    return _fragmentsModel;
}

/*!
 * \brief CMediaBase::saveDb
 * \return
 */
bool CMediaBase::BASE_saveData()
{
    qDebug() << "Saving data to Database:";
    bool fragmentsflag = true, playlistsflag = true;
    qDebug() << "> Playlists:";
    for(auto i = _playlists->cbegin() + 1; i < _playlists->cend(); i++)
    {
        if(!savePlaylist(&*i)) playlistsflag = false;
        qDebug() << ">> " << i->title;
    }
    qDebug() << "> Fragments:";
    for(auto i = _fragments->cbegin(); i < _fragments->cend(); i++)
    {
        if(!saveFragment(&*i)) fragmentsflag = false;
        qDebug() << ">> " << i->title();
    }
    return fragmentsflag && playlistsflag;
}

void CMediaBase::BASE_changeFragmentsList(QItemSelection selected, QItemSelection deselected)
{
     const auto playlistRow = selected.indexes().first().row();
     CMediaPlaylist& newPlaylist = (*_playlists)[playlistRow];
     _fragmentsModel->setListPointer(newPlaylist.getList());
}

/*!
 * \brief Clears database and sets its structure up
 * \b {Returns:} \c true if process was succesful; \c false it it wasn't possible
 */
bool CMediaBase::clearDatabase()
{
    qDebug() << "Clearing database";
    // opening transaction
    if(!isLoaded() || !_database->transaction()) return false;
    // query
    QSqlQuery query;
    // droping tables
    qDebug() << "> Dropping table: " <<
                query.exec("DROP TABLE IF EXISTS fragments, playlists");
    // creating table fragments
    qDebug() << "> Creating fragments table: " <<
                query.exec("CREATE TABLE IF NOT EXISTS fragments ("
               "frag_id INTEGER NOT NULL UNIQUE,"
               "path TEXT NOT NULL,"
               "created INT NOT NULL,"
               "fsize INT NOT NULL,"
               "title TEXT NOT NULL,"
               "start INT NOT NULL,"
               "end INT NOT NULL,"               
               "desc TEXT,"
               "playlist INT NOT NULL,"
               "playlist_pos INT NOT NULL,"
               "PRIMARY KEY(frag_id),"
               "FOREIGN KEY(playlist) REFERENCES playlists(playlists_id))");
    // creating table playlists
    qDebug() << "> Creating playlists table: " <<
                query.exec("CREATE TABLE IF NOT EXISTS playlists ("
               "playlist_id INT NOT NULL UNIQUE,"
               "title TEXT NOT NULL,"
               "desc TEXT,"
               "PRIMARY KEY(playlist_id))");
    // executing
    qDebug() << "> Committing changes.";
    return _database->commit();
}

void CMediaBase::loadFragments(QSqlQuery *query)
{
    qDebug() << "Loading Fragments from Database";
    // loading strings of files in user's directories
    auto filesList = getFilesList();
    // qDebug() << filesList;
    query->exec("SELECT * FROM fragments");
    // building file objects basing on Database
    CMediaFragment* file;
    while(query->next())
    {
        // lets assume that everything is ok
        bool flag = true;
        const auto id = query->value(0).toInt(); // file id
        const auto path = query->value(1).toString(); // file path
        const auto size = query->value(3).toInt();  // file size
        // getting date of file modification (creation)
        quint64 created = query->value(2).toInt(); // date of modification
        // try to create a fragment object basing on database
        try
        {            
            file = new CMediaFragment(id, path, created, size);
            qDebug() << "> Considering file: " << file->file().absoluteFilePath();
        }
        catch(EMediaFragmentError e)
        {
            // if file description from database is not compliant with files list, then asimilate
            if(e == INVALID) flag = asimilation(file, id, size, created, filesList);
                else continue;
            if(!flag) continue; // if asimilation was not successful
            // in case of adding signals about problems
        }
        // filling remaining data
        file->setStart(query->value(5).toInt()); // fragment start
        file->setEnd(query->value(6).toInt()); // fragment end
        file->setTitle(query->value(4).toString()); // fragment title
        file->setDesc(query->value(7).toString()); // fragment description
        // adding to playlist
        int playlist_id = query->value(8).toInt(); // playlist id
        int playlist_pos = query->value(9).toInt(); // position on playlist
        // look for playlist of provided id
        auto playlist = std::find_if(_playlists->begin(), _playlists->end(),
                                 [&playlist_id](auto list)->bool{ return playlist_id == list.id(); });
        // when file object is ok and is located in Directories, then add it to Files
        if(playlist != _playlists->end() && flag && std::any_of(_directoriesPtr->cbegin(), _directoriesPtr->cend(),
                               [&file](const QDir directory)->bool{ return  file->file().absoluteFilePath().contains(directory.absolutePath()); }))
        {
            _fragments->append(*file);
            playlist->addFragment(&_fragments->back(), playlist_pos);
            // remove *file from filesList
            filesList.removeAll(file->file());
            qDebug() << ">> Fragment added to Fragments list";
        }
        // delete file, as it was copied to to list or not used
        delete file;
    }
    // adding remaining files from filesList to _files
    qDebug() << "> Adding remaining or newly found files:";
    foreach (auto i, filesList) {
        file = newMediaFile(i);
        if(file)
        {
            // file
            qDebug() << ">> " << file->file().absoluteFilePath();
            _fragments->append(*file);
            // first, default playlist
            CMediaPlaylist& default_playlist = _playlists->first(); // getting first playlist - default
            int position = default_playlist.size() + 1; // setting position for fragment
            default_playlist.addFragment(&_fragments->last(), position); // adding
            delete file;
        }
    }
    qDebug() << "> Fragments loaded";
}

bool CMediaBase::loadPlaylists(QSqlQuery *query)
{
    // playlists
    qDebug() << "Loading playlists";
    // creating default playlist of all fragments
    _playlists->append(CMediaPlaylist(0, "Wszystkie utwory", "Wszystkie utwory"));
    qDebug() << "> Default playlists added";
    // getting playlists from db
    query->exec("SELECT * FROM playlists");
    while (query->next()) {
        auto playlist = CMediaPlaylist(query->value(0).toInt(), query->value(1).toString(), query->value(2).toString());
        if(!playlist.id()) continue; // if first playlist then continue
        qDebug() << ">> Considering playlist: " << playlist.title;
        _playlists->append(playlist);
    }
    qDebug() << "PLAYLISTS loaded";
}

/*!
 * \brief CMediaBase::asimilation
 */
bool CMediaBase::asimilation(CMediaFragment* file, const int& id, const int& size, const quint64 created, const QFileInfoList& dirs) const
{
    qDebug() << "> File asimilation";
    // find file
    auto newFile = std::find_if(dirs.cbegin(), dirs.cend(),
                                [&size, &created](QFileInfo file)->bool{ return size == file.size() && created == file.created().currentSecsSinceEpoch(); });
    // insert it to new file or return false
    if(newFile == dirs.cend()) return false;
    else
    {
        qDebug() << ">> Successful";
        file = new CMediaFragment(id, *newFile);
        return true;
    }
}

