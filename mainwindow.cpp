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
    base(new CMediaBase(this)) // base initialization
{
    qDebug() << "Initializing main window..";
    ui->setupUi(this);

    // connections
    qDebug() << "> Setting connections";
    connect(settings, SIGNAL(SETT_createDB(const QFileInfo, const QList<QDir>*, const QStringList*)), base, SLOT(BASE_createDatabase(const QFileInfo, const QList<QDir>*, const QStringList*))); // demand to create dbs
    connect(settings, SIGNAL(SETT_loadDB(const QFileInfo, const QList<QDir>*, const QStringList*)), base, SLOT(BASE_loadDatabase(const QFileInfo, const QList<QDir>*, const QStringList*))); // demand to load dbs
    connect(settings, SIGNAL(SETT_update()), this, SLOT(WIND_updateSettings())); // updating settings with window parameters
    connect(base, SIGNAL(BASE_DatabaseLoaded(const bool)), settings, SLOT(SETT_DBLoadResult(const bool))); // notification to settings about dbs load
    connect(base, SIGNAL(BASE_DatabaseCreated(const QFileInfo)), settings, SLOT(SETT_DBCreateResult(const QFileInfo))); // notification to settings about dbs create

    /// prepare adding and removing rows in runtime
    // settings init
    settings->Init();

    // setting window properties
    move(settings->windowPos()); // setting window to last position on screen
    resize(settings->windowSize()); // setting window to last size
    setMinimumSize(settings->_minWindowSize); // setting minimum size of window

    // setting models
    ui->playlists_listView->setModel(base->getPlaylistsModel().get());
    ui->fragments_listView->setModel(base->getFragmentsModel().get());

    // connections between models and controls
    connect(ui->playlists_listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(WIND_enableDeleteButton(QItemSelection, QItemSelection))); // enabling delete button
    connect(ui->playlists_listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), base, SLOT(BASE_changeFragmentsList(QItemSelection, QItemSelection))); // changing list in fragments model


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
void MainWindow::WIND_DBErrorNotify(const bool loaded, const QFileInfo localisation) const
{
	if(!loaded)
	{
		QMessageBox notify;
		notify.setText("Nie znaleziono bazy danych w podanej lokalizacji: ");
		notify.setInformativeText(localisation.absoluteFilePath() + "\nSprawdź ustawienia.");
        notify.setStandardButtons(QMessageBox::Ok);
		notify.setIcon(QMessageBox::Warning);
		notify.exec();
    }
}

void MainWindow::WIND_updateSettings()
{
    qDebug() << "Updating settings- Main Window";
    settings->setWindowPos(pos()); // setting window position
    settings->setWindowSize(size()); // setting current window size
}

void MainWindow::WIND_enableDeleteButton(QItemSelection selected, QItemSelection deselected)
{
    // getting first
    auto first = ui->playlists_listView->model()->index(0, 0);
    ui->deletePushButton->setEnabled((selected.contains(first)) ? false : true);
}

void MainWindow::on_addPushButton_clicked()
{
    // adding new playlist
    qDebug() << "> Add button clicked";
    const auto count = ui->playlists_listView->model()->rowCount(); // getting count of elements in model
    ui->playlists_listView->model()->insertRow(count); // appending new row

    /*
    // activating last to edit
    const auto index = ui->playlists_listView->model()->index(count, 0);
    ui->playlists_listView->edit(index);
    */
}

void MainWindow::on_deletePushButton_clicked()
{
    // deleting selected playlist
    qDebug() << "> Delete button clicked";

    // getting indexes
    auto indexes = ui->playlists_listView->selectionModel()->selectedIndexes();

    // sorting
    std::sort(indexes.begin(), indexes.end(), [](const QModelIndex a, const QModelIndex b)->bool{ return a.row() >= b.row(); });

    // removing
    foreach (auto index, indexes) {
        ui->playlists_listView->model()->removeRow(index.row());
    }
    // maybe use setUpdatesEnabled? YOURVIEW->setUpdatesEnabled(false);
}
