#include "dbwizardpage.h"
#include "ui_dbwizardpage.h"

/*!
  \class dbWizardPage
  \inmodule Settings Wizard
  \brief \l SettingsWizard page for collecting data for database creation.
  \target Fields
  \chapter Fields
  It has following \b {fields}:
  \list
    \li database
   \endlist
  */

/*!
 * Default constructor - register \b {fields} and sets up UI.
 */
dbWizardPage::dbWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::dbWizardPage)
{
    ui->setupUi(this);
    // registering fields
    registerField("database", ui->lineEdit_database);
}

/*!
 * Default destructor - deletes UI
 */
dbWizardPage::~dbWizardPage()
{
    delete ui;
}

/*!
 * \brief Checks whether fields are checked or empty.
 * In order to provide data validation.
 * Returns \c True if \l Fields are completed or not checked. Otherwise, return \c false.
 */
bool dbWizardPage::isComplete() const
{
    return (!ui->groupBox_database->isChecked() || !ui->lineEdit_database->text().isEmpty()) ? true : false;
}

/*!
 * \brief Clears \c playlist field if unchecked.
 */
void dbWizardPage::on_groupBox_database_clicked(bool checked)
{
    if(!checked) ui->lineEdit_database->clear();
    emit completeChanged();
}

/*!
 * \brief Triggered when \c playlists lineEdit was changed.
 * In order to provide data validation.
 */
void dbWizardPage::on_lineEdit_database_textChanged(const QString &arg1)
{
    emit completeChanged();
}

/*!
 * \brief Opens browser for searching where \c playlists database should be created.
 */
void dbWizardPage::on_pushButton_databaseBrowse_clicked()
{
    // open file browser
    ui->lineEdit_database->setText(getMusicDirectory());
}

/*!
 * \brief Opens browser for searching for directory.
 * Opens at default Music location.
 * Return chosen directory
 */
QString dbWizardPage::getMusicDirectory()
{
    return QFileDialog::getExistingDirectory(0, QString(), QStandardPaths::standardLocations(QStandardPaths::MusicLocation).last());
}
