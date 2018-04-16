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
    // settings init
    settings->Init();
    // setting window properties
    move(settings->windowPos()); // setting window to last position on screen
    resize(settings->windowSize()); // setting window to last size
    setMinimumSize(settings->_minWindowSize); // setting minimum size of window
	



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



