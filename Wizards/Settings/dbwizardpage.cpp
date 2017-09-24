#include "dbwizardpage.h"
#include "ui_dbwizardpage.h"

dbWizardPage::dbWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::dbWizardPage)
{
    ui->setupUi(this);
    // registering fields
    registerField("fragments", ui->lineEdit_fragments);
    registerField("playlists", ui->lineEdit_playlists);
}

dbWizardPage::~dbWizardPage()
{
    delete ui;
}

bool dbWizardPage::isComplete() const
{
    return ((!ui->groupBox_fragments->isChecked() || !ui->lineEdit_fragments->text().isEmpty()) &&
            (!ui->groupBox_playlists->isChecked() || !ui->lineEdit_playlists->text().isEmpty())) ? true : false;
}

void dbWizardPage::on_groupBox_playlists_clicked(bool checked)
{
    if(!checked) ui->lineEdit_playlists->clear();
    emit completeChanged();
}

void dbWizardPage::on_groupBox_fragments_clicked(bool checked)
{
    if(!checked) ui->lineEdit_fragments->clear();
    emit completeChanged();
}

void dbWizardPage::on_lineEdit_fragments_textChanged(const QString &arg1)
{
    emit completeChanged();
}

void dbWizardPage::on_lineEdit_playlists_textChanged(const QString &arg1)
{
    emit completeChanged();
}

void dbWizardPage::on_pushButton_playlistsBrowse_clicked()
{
    // open file browser
    ui->lineEdit_playlists->setText(getMusicDirectory());
}

void dbWizardPage::on_pushButton_fragmentsBrowse_clicked()
{
    // open file browser
    ui->lineEdit_fragments->setText(getMusicDirectory());
}

QString dbWizardPage::getMusicDirectory()
{
    return QFileDialog::getExistingDirectory(0, QString(), QStandardPaths::standardLocations(QStandardPaths::MusicLocation).last());
}
