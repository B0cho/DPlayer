#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // inits
    settings = new CSettings(this);
	base = new CMediaBase(this);
    // connections
    connect(settings, SIGNAL(createDBs(const QFileInfo, const QFileInfo )), base, SLOT(createDatabases(const QFileInfo, const QFileInfo ))); // demand to create dbs
    connect(settings, SIGNAL(loadDBs(const QFileInfo, const QFileInfo )), base, SLOT(loadDatabases(const QFileInfo, const QFileInfo))); // demand to load dbs
    connect(base, SIGNAL(DatabasesLoaded(const bool, const bool)), settings, SLOT(DBsLoadResult(const bool, const bool))); // notification to settings about dbs load
    connect(base, SIGNAL(DatabasesCreated(const QFileInfo, const QFileInfo)), settings, SLOT(DBsCreateResult(const QFileInfo, const QFileInfo))); // notification to settings about dbs create
    // settings init
    settings->Init();
	
	



}

MainWindow::~MainWindow()
{


}

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



