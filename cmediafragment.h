#ifndef CMEDIAFILE_H
#define CMEDIAFILE_H

#include <QFileInfo>
#include <QDateTime>
#include <QDebug>
#include "cdatabasemember.h"

enum EMediaFragmentError
{
    INVALID,
    NO_READABLE,
    NO_FILE
};

class CMediaFragment : public CDatabaseMember
{
public:
    CMediaFragment(const int id, const QString file_path, const quint64 created, const qint64 file_size);
    CMediaFragment(const int id, const QFileInfo file_info);
    QFileInfo file() const;

    QString title() const;
    void setTitle(const QString title);

    qint16 start() const;
    void setStart(const qint16 start);

    qint16 end() const;
    void setEnd(const qint16 end);

    QString desc() const;
    void setDesc(const QString desc);

private:
    QFileInfo _file;
    QString _title;
    qint16 _start;
    qint16 _end;
    QString _desc;
};

#endif // CMEDIAFILE_H


