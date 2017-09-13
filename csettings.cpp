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
    if(!isRegKey("")) // launch wizard

}

const bool CSettings::isRegKey(const QString key) const
{

}
