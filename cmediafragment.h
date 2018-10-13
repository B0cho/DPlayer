#ifndef CMEDIAFILE_H
#define CMEDIAFILE_H

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include "cdatabasemember.h"

/// FRAGMENT ERROR ENUM
enum EMediaFragmentError
{
    INVALID,
    NO_READABLE,
    NO_FILE
};

/// REPRESENTS FRAGMENT
class CMediaFragment : public CDatabaseMember
{
public:
    /// ctors
    CMediaFragment(const int id, const QString file_path, const quint64 created, const qint64 file_size);
    CMediaFragment(const int id, const QFileInfo file_info);
    CMediaFragment(const int id, const CMediaFragment& toBeCopied);

    /// methods, getters and setters
    QFileInfo file() const;

    QString title() const;
    void setTitle(const QString title);

    qint16 start() const;
    bool setStart(const qint16 start);

    qint16 end() const;
    bool setEnd(const qint16 end);

    QString desc() const;
    void setDesc(const QString desc);

    /// scope of fragment
    static const qint16 startScope = 0;
    static const qint16 endScope = 1000;

private:
    /// members
    QFileInfo _file; // original file
    QString _title; // fragment title
    qint16 _start; // 0 - 1000
    qint16 _end; // 0 - 1000
    QString _desc; // description
};

#endif // CMEDIAFILE_H


