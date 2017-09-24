#include "dirswizardpage.h"
#include "ui_dirswizardpage.h"

dirsWizardPage::dirsWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::dirsWizardPage)
{
    ui->setupUi(this);
    // adding default directory
    ui->listWidget_directories->addItem(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).last());
    ui->pushButton_deleteDirectory->setEnabled(false);
}

dirsWizardPage::~dirsWizardPage()
{
    delete ui;
}

void dirsWizardPage::on_pushButton_addDirectory_clicked()
{
    ui->listWidget_directories->addItem(dbWizardPage::getMusicDirectory());
}

void dirsWizardPage::on_listWidget_directories_itemClicked(QListWidgetItem *item)
{
    ui->pushButton_deleteDirectory->setEnabled(true);
}

void dirsWizardPage::on_pushButton_deleteDirectory_clicked()
{
    auto p = ui->listWidget_directories;
    p->takeItem(p->currentRow());
    p->setCurrentRow(-1);
    ui->pushButton_deleteDirectory->setEnabled(false);
}

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
