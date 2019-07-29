#ifndef SETTINGSWIZARD_H
#define SETTINGSWIZARD_H

#include <QWizard>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

namespace Ui {
class SettingsWizard;
}

struct settingsWizardData
{
    QString databaseDirectory;
    QList<QString> mediaDirectories;
    //
};

struct settingsWizardFeedback
{
    bool regkeyCreated;
    QString databasePath;
    //
};

class SettingsWizard : public QWizard
{
    Q_OBJECT

public:
    // ctor and dtor
    explicit SettingsWizard(QWidget *parent = 0);
    ~SettingsWizard();
signals:
    void WIZZ_dataCompleted(settingsWizardData data) const;
private:
    Ui::SettingsWizard *ui;
public slots:
    void WIZZ_wizardFeedback(const settingsWizardFeedback results);
private slots:
    void on_SettingsWizard_currentIdChanged(int id);
};

#endif // SETTINGSWIZARD_H
