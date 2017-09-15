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
    connect(settings, SIGNAL(launchSettingsWizard()), this, SLOT(settingsWizardInit()));
	connect(settings, SIGNAL(createDB(const QDir)), base, SLOT(createDatabase(const QDir)));
	connect(settings, SIGNAL(loadDB(const QFileInfo)), base, SLOT(loadDatabase(const QFileInfo)));
	connect(base, SIGNAL(DatabaseLoaded(const bool)), settings, SLOT(DBloadNotify(const bool)));
	connect(base, SIGNAL(DatabaseCreated(const bool, const QFileInfo)), settings, SLOT(DBloadNotify(const bool, const QFileInfo)));
	connect(base, SIGNAL(DatabaseCreated(const bool, const QFileInfo)), this, SLOT(DBErrorNotify(const bool, const QFileInfo)));
    // settings init
    settings->Init();
	
	



}

MainWindow::~MainWindow()
{


}

void MainWindow::DBErrorNotify(const bool loaded, const QFileInfo localisation)
{
	if(!loaded)
	{
		QMessageBox notify;
		notify.setText("Nie znaleziono bazy danych w podanej lokalizacji: ");
		notify.setInformativeText(localisation.absoluteFilePath() + "\nSprawdź ustawienia.");
		notify.setStandardButtons(QMessageBox::OK);
		notify.setIcon(QMessageBox::Warning);
		notify.exec();
	}	
}



