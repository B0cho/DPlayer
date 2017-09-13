#ifndef SETTINGSWIZARD_H
#define SETTINGSWIZARD_H

#include <QWizard>

namespace Ui {
class SettingsWizard;
}

class SettingsWizard : public QWizard
{
    Q_OBJECT

public:
    explicit SettingsWizard(QWidget *parent = 0);
    ~SettingsWizard();

private:
    Ui::SettingsWizard *ui;
};

#endif // SETTINGSWIZARD_H
