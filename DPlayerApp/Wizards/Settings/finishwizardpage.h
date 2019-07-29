#ifndef FINISHWIZARDPAGE_H
#define FINISHWIZARDPAGE_H

#include <QWizardPage>

namespace Ui {
class finishWizardPage;
}

class finishWizardPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit finishWizardPage(QWidget *parent = 0);
    ~finishWizardPage();

private:
    Ui::finishWizardPage *ui;
    void initializePage();
};

#endif // FINISHWIZARDPAGE_H
