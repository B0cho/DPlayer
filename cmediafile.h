#ifndef CMEDIAFILE_H
#define CMEDIAFILE_H

#include <QFileInfo>
#include <QDateTime>
#include "cdatabasemember.h"

enum EMediaFileError
{
    INVALID,
    NO_READABLE,
    NO_FILE
};

class CMediaFile : public CDatabaseMember
{
public:
    CMediaFile(const int id, const QString file_path, const qint64 file_size, const QDateTime created, const QString file_genre = "");
    QFileInfo file() const;

private:
    QFileInfo _file;
    QString _genre;
    QDateTime _created;
    qint64 _size;
};

#endif // CMEDIAFILE_H
