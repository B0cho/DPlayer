#include "settingswizard.h"
#include "ui_settingswizard.h"

SettingsWizard::SettingsWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SettingsWizard)
{
    ui->setupUi(this);
    // wizard window flags init
    auto flags = windowFlags();
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    // settings pixmap
    //_watermark.load()
    //setPixmap(QWizard::WatermarkPixmap, _watermark);
}

SettingsWizard::~SettingsWizard()
{
    delete ui;
}


const QList<QDir> SettingsWizard::getPaths() const
{
	
	
}
