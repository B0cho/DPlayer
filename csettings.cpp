#include "csettings.h"

CSettings::CSettings(QObject *parent) : QObject(parent)
{

}

CSettings::~CSettings()
{

}

void CSettings::Init(const QString organisation, const QString appName)
{
    // checking whether register key exists


    // if not
    emit launchSettingsWizard();

}
