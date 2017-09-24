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

struct settingsWizardData
{
    QString fragmentsDirectory, playlistsDirectory;
    QList<QString> mediaDirectories;
    QPair<QString, QString> ytData;
};

struct settingsWizardFeedback
{
    bool regkeyCreated;
    QString playlistsPath, fragmentsPath;

};

class SettingsWizard : public QWizard
{
    Q_OBJECT

public:
    explicit SettingsWizard(QWidget *parent = 0);
    ~SettingsWizard();
	
	// methods

signals:
    void dataCompleted(settingsWizardData data) const;

private:
    Ui::SettingsWizard *ui;

public slots:
    void wizardFeedback(const settingsWizardFeedback results);

private slots:
    void on_SettingsWizard_currentIdChanged(int id);
};

#endif // SETTINGSWIZARD_H
