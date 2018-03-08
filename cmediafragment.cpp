#include "cmediafragment.h"

CMediaFragment::CMediaFragment(const int id, const CMediaFile* file, const QTime start, const QTime end, const QDateTime created, const QString title, const QString description)
    : CDatabaseMember(id), _file(file), _start(start), _end(end), _created(created), _title(title), _desc(description)
{

}
