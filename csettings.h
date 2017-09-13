#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QWizard>
#include <QString>
#include <QUrl>
#include <QDateTime>
#include <QVariant>
#include "Wizards/settingswizard.h"
//
class CSettings : public QObject
{
    Q_OBJECT
public:
    // ctors dtors
    explicit CSettings(QObject *parent = nullptr);
    ~CSettings();
    void Init(const QString organisation, const QString appName);
private:
    // members
    QSettings _reg;
    QUrl _lastdir;
    QDateTime _lastDate, _creationDate;


    // methods


signals:
    void launchSettingsWizard() const;


public slots:
};

#endif // CSETTINGS_H
