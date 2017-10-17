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
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // inits
    settings = new CSettings(this);
	base = new CMediaBase(this);
    // connections
    connect(settings, SIGNAL(createDB(const QFileInfo)), base, SLOT(createDatabase(const QFileInfo))); // demand to create dbs
    connect(settings, SIGNAL(loadDB(const QFileInfo)), base, SLOT(loadDatabase(const QFileInfo))); // demand to load dbs
    connect(base, SIGNAL(DatabaseLoaded(const bool)), settings, SLOT(DBLoadResult(const bool))); // notification to settings about dbs load
    connect(base, SIGNAL(DatabaseCreated(const QFileInfo)), settings, SLOT(DBCreateResult(const QFileInfo))); // notification to settings about dbs create
    // settings init
    settings->Init();
	
	



}

/*!
 * \brief Default destructor
 * */

MainWindow::~MainWindow()
{


}

/*!
 * \brief MainWindow::DBErrorNotify
 *
 *
 */
void MainWindow::DBErrorNotify(const bool loaded, const QFileInfo localisation) const
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



