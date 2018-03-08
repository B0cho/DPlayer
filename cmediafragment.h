#ifndef CMEDIAFRAGMENT_H
#define CMEDIAFRAGMENT_H

#include "cmediafile.h"

class CMediaFragment : public CDatabaseMember
{
public:
    CMediaFragment(const int id, const CMediaFile* file, const QTime start, const QTime end, const QDateTime created, const QString title, const QString description = "");
private:
    const CMediaFile* _file;
    QDateTime _created;
    QTime _start, _end;
    QString _title, _desc;
};

#endif // CMEDIAFRAGMENT_H

