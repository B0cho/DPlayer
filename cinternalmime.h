#ifndef CINTERNALMIME_H
#define CINTERNALMIME_H

#include <QObject>
#include <QMimeData>

/// MIME-DATA CLASS-TEMPLATE
template <class T>
class CInternalMime : public QMimeData
{
private:
    const QString mimeType; // qstring to identify mimeType
public:
    CInternalMime(const QString type);
    QList<const T*> container; // main container of data

    /// statics
    constexpr static QString fragmentMimeType, playlistMimeType; // statics that may be used to be passed as type for CMediaFragment and CMediaPlaylisy

    /// QMIMEDATA
    bool hasFormat(const QString &mimetype) const;
    QStringList formats() const;
};

template <class T>
const QString CInternalMime<T>::fragmentMimeType = "fragmentMime";

template <class T>
const QString CInternalMime<T>::playlistMimeType = "playlistMime";

template <class T>
CInternalMime<T>::CInternalMime(const QString type) : mimeType(type)
{

}

template<class T>
bool CInternalMime<T>::hasFormat(const QString &mimetype) const
{
    return mimetype == mimeType;
}

template<class T>
QStringList CInternalMime<T>::formats() const
{
    QStringList mimeTypes;
    mimeTypes << mimeType;
    return mimeTypes;
}

#endif // CINTERNALMIME_H
