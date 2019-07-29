#include "settingswizard.h"
#include "ui_settingswizard.h"

/*!
  \module Initialization
  This module contains all things that are in charge of app startup and configuration
  */
/*!
  \module Settings Wizard
  This module contains all things that \l SettingsWizard consists of.
  */
/*!
 * \class SettingsWizard class
 * \inmodule Initialization
 * \inmodule Settings Wizard
 * \brief {The SettingsWizard class provides wizard to get initial information about preferences and basic paths from user.}
 * This wizard has following pages:
 * \table
 * \header
 *  \li Page class
 *  \li Page description
 * \row
 *  \li QWizardPage
 *  \li \l {intro_page} {Intro page}
 * \row
 *  \li \l dbWizardPage
 *  \li Databases page
 * \row
 *  \li \l dirsWizardPage
 *  \li Directories page
 * \row
 *  \li QWizardPage
 *  \li \l {finish_page} {Finish page}
 * \endtable
 * where first and last are nested in wizard QML.
 *
 * Youtube account page is prepared to provide YT account login in further versions of DPlayer.
 *
 * Wizard window is intended to be launched as \b {modal} at first application launch.
 * Thanks to data routing with slots and signals, is completely separated from its host.
 * Right after settingsWizard creation, two \b {signals and slots} pairs must be connected:
 * \list
 *  \li \l {WIZZ_dataCompleted(settingsWizardData data)} signal
 *  \li \l {WIZZ_wizardFeedback(const settingsWizardFeedback results)} slot
 * \endlist
 * Data exchange is provided through \l settingsWizardData and \l settingsWizardFeedback structs.
 * \sa settingsWizardData
 * \sa settingsWizardFeedback
 * \note To process wizard, use \b {exec()}.
 * If user closes wizard using \c Cancel, no \l WIZZ_dataCompleted(settingsWizardData data) signal is emitted.
 *
 * \target intro_page
 * \chapter Intro Page
 * This page is a welcome page, nothing more :) Stored in SettingsWizard QML has a few Qlabel widgets.
 *
 * \target finish_page
 * \chapter Finish page
 * This page contains several QLabels- their content is adjusted by \l WIZZ_wizardFeedback slot, depending on \a feedback it takes.
 * Stored in SettingsWizard QML, informs user about result of operations of creating \l {playlistsDatabase} and/or \l {fragmentsDatabase} and so on.
 */

/*!
 * Default constructor, which prepares window to be shown- proper flags and pixmaps are being set.
 */

SettingsWizard::SettingsWizard(QWidget *parent) :
    QWizard(parent),
    ui(new Ui::SettingsWizard)
{
    qDebug() << "Initializing Wizard";
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

/*!
 * \brief Default destructor.
 */

SettingsWizard::~SettingsWizard()
{
    delete ui;
}

/*!
 * Slot, which takes \l{settingsWizardFeedback} struct \a results and adjusts \l {finish_page} {last wizard page}.
 * If any of \a results members is \c{false}, then a propriate information is given to the user in a way defined in this method-
 * normally as descriptions sent to page labels.
 */

void SettingsWizard::WIZZ_wizardFeedback(const settingsWizardFeedback results)
{
    qDebug() << "WIZZ_wizardFeedback fired";
    QString label("");
    // registry info
    label = (results.regkeyCreated) ? "Registry key for settings created." : "Failed to create registry key";
    ui->label_regstry->setText(label);
    label.clear();
    // fragments info
    label += (QFileInfo(results.databasePath).isFile()) ? "Database file was created as " + results.databasePath
                                                         : "Failed to create database file in provided directory!";
    label += "\n\n";
    ui->label_databases->setText(label);
}

/*!
 * This slot is executed at every page change demand.
 * It's role is to check whether \l {finish_page} {last page} is going to be load and if so, emits \l {SettingsWizard::WIZZ_dataCompleted(settingsWizardData data)}{WIZZ_dataCompleted(settingsWizardData) signal}
 * with data prvided to wizard, stored in \l settingsWizardData struct.
 *
 * \sa slot
 */

void SettingsWizard::on_SettingsWizard_currentIdChanged(int id)
{
    // if last page is going to be loaded
    qDebug() << "on_SettingsWizard_currentIdChanged fired";
    if(nextId() == -1 && id != -1)
    {
        settingsWizardData data;
        data.databaseDirectory = field("database").toString();
        data.mediaDirectories = ui->dirsPage->getDirectories();
        //
        emit WIZZ_dataCompleted(data);
    }
}
/*!
  \fn SettingsWizard::WIZZ_dataCompleted(settingsWizardData data) const
  This signal is emitted when user clicks \c Next button at page that is just before \l {finish_page}{finish page} and passess collected \c data.
  \sa settingsWizardData
 */

/*!
  \class settingsWizardData
  \inmodule Initialization
  \brief This struct is used to pass data collected by \l SettingsWizard to its host
  \sa SettingsWizard
  \sa settingsWizardFeedback
  \l SettingsWizard uses two structs to exchange data with its host in order to provide \b {wizard - host} isolation.
  When user has provided all required data, \l SettingsWizard emits \l {SettingsWizard::WIZZ_dataCompleted(settingsWizardData data)}{WIZZ_dataCompleted(settingsWizardData) signal}, which takes an argument of described type.
  The struct has following members:
  */
/*!
  \variable settingsWizardData::fragmentsDirectory
  \brief This member holds directory where \l {fragmentsDatabase}{fragments database} should be created.
  By default, \b empty \a fragmentsDirectory means that no database should be created.
  \sa SettingsWizard::WIZZ_dataCompleted
  */
/*!
  \variable settingsWizardData::playlistsDirectory
  This member holds directory where \l {playlistsDatabase}{playlists database} should be created.
  By default, \b empty \a playlistsDirectory means that no database should be created.
  \sa SettingsWizard::WIZZ_dataCompleted
*/
/*!
  \variable settingsWizardData::mediaDirectories
  Contains directories where are placed our media files.
  */
/*!
  \variable settingsWizardData::ytData
  QPair to contain Youtube account login and password.
  \note This is not completed! This variable is planned to be used in further app developments
  */

/*!
  \class settingsWizardFeedback
  \inmodule Initialization
  \brief This struct is used to pass results of processes melting \l SettingsWizard user data.
  \sa SettingsWizard
  \sa settingsWizardData
   It gives feedback to \l SettingsWizard, so it can adjust \l {finish_page}{finish page} to inform user about results.
*/
/*!
  \variable settingsWizardFeedback::regkeyCreated
  Returns \c true if creation of \l {regkey}{registry key} was successful.
*/
/*!
 * \variable settingsWizardFeedback::fragmentsPath
 * Contains path to created \l {fragmentsDatabase}{fragments database} path.
 * \note If no database was created, variable must contain \b empty QString! It is crucial for \l settingsWizardFeedback slot!
 * */
/*!
 * \variable settingsWizardFeedback::playlistsPath
 * Contains path to created \l {playlistsDatabase}{playlists database} path.
 * \note If no database was created, variable must contain \b empty QString! It is curcial for \l settingsWizardFeedback slot!
 * */
