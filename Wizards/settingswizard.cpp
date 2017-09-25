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

void SettingsWizard::wizardFeedback(const settingsWizardFeedback results)
{
    QString label("");
    // registry info
    label = (results.regkeyCreated) ? "Registry key for settings created." : "Failed to create registry key";
    ui->label_regstry->setText(label);
    label.clear();
    // fragments info
    label += (QFileInfo(results.fragmentsPath).isFile()) ? "Fragments database file was created as " + results.fragmentsPath
                                                         : "Failed to create fragments database file in provided directory!";
    label += '\n\n';
    // playlists info
    label += (QFileInfo(results.playlistsPath).isFile()) ? "Playlists database file was created as " + results.playlistsPath
                                                         : "Failed to create playlists database file in provided directory!";
    ui->label_databases->setText(label);
}

void SettingsWizard::on_SettingsWizard_currentIdChanged(int id)
{
    // if last page is going to be loaded
    if(nextId() == -1 && currentId() != -1)
    {
        settingsWizardData data;
        data.fragmentsDirectory = field("fragments").toString();
        data.playlistsDirectory = field("playlists").toString();
        data.mediaDirectories = ui->dirsPage->getDirectories();
        //
        emit dataCompleted(data);
    }
}
