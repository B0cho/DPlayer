#include "settingswizard.h"
#include "ui_settingswizard.h"

SettingsWizard::SettingsWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SettingsWizard)
{
    ui->setupUi(this);
    // setting fixed size
    setFixedSize(width(), height());
    // wizard window flags init
    auto flags = windowFlags();
    flags &= ~Qt::WindowContextHelpButtonHint;
    flags &= ~Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    // settings pixmap
    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/wizards/init/pendo").scaled(230, 300));
    setPixmap(QWizard::LogoPixmap, QPixmap(":/wizards/init/pepe").scaled(100, 100));
}

SettingsWizard::~SettingsWizard()
{
    delete ui;
}


const QList<QDir> SettingsWizard::getPaths() const
{
	
	
}
