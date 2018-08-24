#ifndef CFRAGMENTMIME_H
#define CFRAGMENTMIME_H

#include <QObject>
#include <QMimeData>
#include "cmediafragment.h"

/// MIME-DATA CLASS FOR FRAGMENT -> template!!
typedef QList<CMediaFragment> CMFragmentsQList;
class CFragmentMime : public QMimeData
{    
public:
    CFragmentMime();
    const static QString mimeType;
    CMFragmentsQList fragments;

    /// QMIMEDATA
    bool hasFormat(const QString &mimetype) const;
    QStringList formats() const;
};

#endif // CFRAGMENTMIME_H
