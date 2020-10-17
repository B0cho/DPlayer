#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
  \class MainWindow
  \brief Class for app main window
  \inmodule Initialization
  Contain all necessary members to hold data and state.

  */

/*!
 * \brief MainWindow::MainWindow
 * Default class constructor- inits UI, but also settings and media base, binds all proper signals and slots between components.
 * \sa CSettings
 * \sa CMediaBase
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings(new CSettings(this)), // settings initialization
    base(new CMediaBase(this)), // base initialization
    searchWidget(new CSearchWidget(this)), // search widget init
    playerWidget(new CPlayerWidget(this)) // player widget init
{
    qDebug() << "Initializing main window..";
    ui->setupUi(this);

    // adding custom widgets
    // search widget
    ui->horizontalLayout->insertWidget(0, searchWidget.get()); // adding to layout
    searchWidget->show(); // showing
    // player widget
    ui->verticalLayout->insertWidget(4, playerWidget.get());
    playerWidget->show();

    // connections
    qDebug() << "> Setting connections";
    connect(settings, SIGNAL(SETT_createDB(const QFileInfo, const QList<QDir>*, const QStringList*)), base, SLOT(BASE_createDatabase(const QFileInfo, const QList<QDir>*, const QStringList*))); // demand to create dbs
    connect(settings, SIGNAL(SETT_loadDB(const QFileInfo, const QList<QDir>*, const QStringList*)), base, SLOT(BASE_loadDatabase(const QFileInfo, const QList<QDir>*, const QStringList*))); // demand to load dbs
    connect(settings, SIGNAL(SETT_update()), this, SLOT(WIND_updateSettings())); // updating settings with window parameters
    connect(base, SIGNAL(BASE_DatabaseLoaded(const bool)), settings, SLOT(SETT_DBLoadResult(const bool))); // notification to settings about dbs load
    connect(base, SIGNAL(BASE_DatabaseLoaded(const bool)), this, SLOT(WIND_DBErrorNotify(const bool)));
    connect(base, SIGNAL(BASE_DatabaseCreated(const QFileInfo)), settings, SLOT(SETT_DBCreateResult(const QFileInfo))); // notification to settings about dbs create

    // settings init
    settings->Init();

    // setting window properties
    move(settings->windowPos()); // setting window to last position on screen
    resize(settings->windowSize()); // setting window to last size
    setMinimumSize(settings->_minWindowSize); // setting minimum size of window

    // setting models
    ui->playlists_listView->setModel(base->getPlaylistsModel().get());
    ui->fragments_listView->setModel(base->getFragmentsModel().get());
    searchWidget->setModel(new CReadOnlyCombinedModel({base->getAllFragmentsModel(), base->getPlaylistsModel()}));

    // setting delegates
    ui->playlists_listView->setItemDelegate(new CPlaylistDelegate);
    ui->fragments_listView->setItemDelegate(new CFragmentDelegate);
    searchWidget->setItemDelegate(new CSearchListDelegate);

    // connections between models, base and controls
    connect(ui->playlists_listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), base, SLOT(BASE_changeFragmentsList(QItemSelection, QItemSelection))); // changing list in fragments model
    connect(ui->dropSiteLabel, SIGNAL(isDeleteAccepted(const QMimeData*,bool&)), base, SLOT(BASE_isDeleteAccepted(const QMimeData*,bool&))); // checks if mime data dropped on dropsite can be deleted
    connect(ui->dropSiteLabel, SIGNAL(droppedMimeData(const QMimeData*)), base, SLOT(BASE_delete(const QMimeData*))); // deleted provided data

    // settings controls
    ui->dropSiteLabel->setFormats({CInternalMime<void>::fragmentMimeType, CInternalMime<void>::playlistMimeType}); // setting formats of dropSite accepted formats
}

/*!
 * \brief Default destructor
 * */

MainWindow::~MainWindow()
{
    WIND_updateSettings();
}

/*!
 * \brief MainWindow::WIND_DBErrorNotify
 *
 *
 */
void MainWindow::WIND_DBErrorNotify(const bool loaded) const
{
	if(!loaded)
	{
        // disabling add button and delete site
        ui->addPushButton->setEnabled(false);
        ui->dropSiteLabel->setEnabled(false);
    }
}

void MainWindow::WIND_updateSettings()
{
    qDebug() << "Updating settings- Main Window";
    settings->setWindowPos(pos()); // setting window position
    settings->setWindowSize(size()); // setting current window size
}

void MainWindow::on_addPushButton_clicked()
{
    // adding new playlist
    qDebug() << "> Add button clicked";
    const auto count = ui->playlists_listView->model()->rowCount(); // getting count of elements in model
    ui->playlists_listView->model()->insertRow(count); // appending new row

    // activating last to edit
    const auto index = ui->playlists_listView->model()->index(count, 0);
    ui->playlists_listView->setCurrentIndex(index);
    ui->playlists_listView->edit(index);

    // saving to database
    base->BASE_saveData();
}
