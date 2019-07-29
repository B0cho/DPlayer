#include "dirswizardpage.h"
#include "ui_dirswizardpage.h"

/*!
 * \class dirsWizardPage
 * \inmodule Settings Wizard
 * \brief \l SettingsWizard page for collecting data about favourite media directories.
 * */

/*!
 * \brief Default constructor
 * Sets up UI and adds default music localisation to \c directories.
 * Enables \c Delete button.
 */
dirsWizardPage::dirsWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::dirsWizardPage)
{
    ui->setupUi(this);
    // adding default directory
    ui->listWidget_directories->addItem(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).last());
    ui->pushButton_deleteDirectory->setEnabled(false);
}

/*!
 * \brief Default destructor
 */
dirsWizardPage::~dirsWizardPage()
{
    delete ui;
}

/*!
 * \brief Lauches broser for directories searching.
 * Selected directory is added to list.
 */
void dirsWizardPage::on_pushButton_addDirectory_clicked()
{
    ui->listWidget_directories->addItem(dbWizardPage::getMusicDirectory());
}

/*!
 * \brief Triggered when directories list element is clicked.
 */
void dirsWizardPage::on_listWidget_directories_itemClicked(QListWidgetItem *item)
{
    ui->pushButton_deleteDirectory->setEnabled(true);
}

/*!
 * \brief Triggered when \c Delete button is clicked.
 */
void dirsWizardPage::on_pushButton_deleteDirectory_clicked()
{
    auto p = ui->listWidget_directories;
    p->takeItem(p->currentRow());
    p->setCurrentRow(-1);
    ui->pushButton_deleteDirectory->setEnabled(false);
}

/*!
 * \brief Returns directories.
 * Returns QStringList of directories in list.
 */
QStringList dirsWizardPage::getDirectories() const
{
    QStringList dirs;
    auto p = ui->listWidget_directories->count();
    for(int i = 0; i < ui->listWidget_directories->count(); i++)
    {
        dirs << ui->listWidget_directories->item(i)->text();
    }
    return dirs;
}
