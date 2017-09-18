#include "settingswizard.h"
#include "ui_settingswizard.h"

SettingsWizard::SettingsWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SettingsWizard)
{
    ui->setupUi(this);

}

SettingsWizard::~SettingsWizard()
{
    delete ui;
}


const QList<QDir> SettingsWizard::getPaths() const
{
	
	
}
