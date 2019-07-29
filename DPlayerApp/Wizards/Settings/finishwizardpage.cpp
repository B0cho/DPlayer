#include "finishwizardpage.h"
#include "ui_finishwizardpage.h"

finishWizardPage::finishWizardPage(QWidget *parent) :
    QWizardPage(parent),
    ui(new Ui::finishWizardPage)
{
    ui->setupUi(this);
}

finishWizardPage::~finishWizardPage()
{
    delete ui;
}

void finishWizardPage::initializePage()
{


}
