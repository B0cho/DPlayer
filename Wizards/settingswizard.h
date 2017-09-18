#ifndef SETTINGSWIZARD_H
#define SETTINGSWIZARD_H

#include <QWizard>
#include <QDir>
#include <QFileInfo>

/*
COMPLETE WIZARD PAGES AND METHODS
CHECK PROPER WORK
*/

namespace Ui {
class SettingsWizard;
}

class SettingsWizard : public QWizard
{
    Q_OBJECT

public:
    explicit SettingsWizard(QWidget *parent = 0);
    ~SettingsWizard();
	
	// methods
    const QList<QDir> getPaths() const;

private:
    Ui::SettingsWizard *ui;
};

#endif // SETTINGSWIZARD_H
