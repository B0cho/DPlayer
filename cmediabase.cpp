#include "cmediabase.h"

CMediaBase::CMediaBase(QObject *parent): QObject(parent)
{
	_loaded = false;
}

CMediaBase::~CMediaBase()
{

}

bool CMediaBase::loadDatabase(const QFileInfo path)
{
	result = true;
	if(path.exists() && path.isWritable() && path.isFile() && path.isReadable() //&& path)
	{
		// loading db
	} else result = false;
	emit DatabaseLoaded(result);
	return _loaded = result;
}

bool CMediaBase::createDatabase(const QDir dir)
{
	if(dir.isAbsolute())
	{
		// creating db
		
	} else return _loaded = false;	
}

const bool CMediaBase::isLoaded()
{
	return _loaded;
}