#include "cfragmentmime.h"

const QString CFragmentMime::mimeType = "fragmentMimeType";

CFragmentMime::CFragmentMime()
{

}

bool CFragmentMime::hasFormat(const QString &mimetype) const
{
    return (mimetype == mimeType) ? true : false;
}

QStringList CFragmentMime::formats() const
{
    QStringList formats;
    formats << mimeType;
    return formats;
}
