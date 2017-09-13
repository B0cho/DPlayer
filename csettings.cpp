#include "csettings.h"

CSettings::CSettings(QObject *parent) : QObject(parent)
{
    // looking for registry keys


    /*
    _reg = QSettings(organisationName.toUpper(), AppName.toUpper());
    lastMedia = _reg.value("lastdir").toUrl();
    _lastTime = _reg.value("lasttime").toDateTime();
    _creationTime = _reg.value("creationtime").toDateTime();
    int i = _reg.beginReadArray("dirs");
    for(int i = 0; i < arraySize; i++)
    {

    }
    */
    // loading db
}

CSettings::~CSettings()
{

}

void CSettings::Init(const QString organisation, const QString appName, const QUrl DBdir)
{
    emit launchSettingsWizard();

}
