#ifndef CFRAGMENTMIME_H
#define CFRAGMENTMIME_H

#include <QObject>
#include <QMimeData>
#include "cmediafragment.h"

/// MIME-DATA CLASS FOR FRAGMENT -> template!!
class CFragmentMime : public QMimeData
{    
public:
    CFragmentMime();
    const static QString mimeType;
    QList<const CMediaFragment*> fragments;

    /// QMIMEDATA
    bool hasFormat(const QString &mimetype) const;
    QStringList formats() const;
};

#endif // CFRAGMENTMIME_H
