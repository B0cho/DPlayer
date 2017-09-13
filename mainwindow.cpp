#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // inits
    settings = new CSettings(this);
    // connections
    connect(settings, SIGNAL(launchSettingsWizard()), this, SLOT(settingsWizardInit()));
    // settings init
    settings->Init(QCoreApplication::organizationName().toUpper(), QCoreApplication::applicationName().toUpper());



}

MainWindow::~MainWindow()
{
    delete ui;
    if(settings) delete settings;

}

void MainWindow::settingsWizardInit()
{
    SettingsWizard* w = new SettingsWizard(this);
    w->exec();


}

