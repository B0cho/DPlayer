#include "cmediafragment.h"

CMediaFragment::CMediaFragment(const int id, const QString file_path, const quint64 created, const qint64 file_size)
    : CDatabaseMember(id), _file(QFileInfo(file_path))
{
    if(!_file.isFile()) throw NO_FILE;
    if(!_file.exists() || _file.size() != file_size || _file.created().toSecsSinceEpoch() != created) throw INVALID;
    if(!_file.isReadable()) throw NO_READABLE;
}

CMediaFragment::CMediaFragment(const int id, const QFileInfo file_info)
    : CDatabaseMember(id), _file(file_info)
{

}

QFileInfo CMediaFragment::file() const
{
    return _file;
}

QString CMediaFragment::title() const
{
    return _title;
}

void CMediaFragment::setTitle(const QString title)
{
    _title = title;
}

qint16 CMediaFragment::start() const
{
    return _start;
}

void CMediaFragment::setStart(const qint16 start)
{
    _start = start;
}

qint16 CMediaFragment::end() const
{
    return _end;
}

void CMediaFragment::setEnd(const qint16 end)
{
    _end = end;
}

QString CMediaFragment::desc() const
{
    return _desc;
}

void CMediaFragment::setDesc(const QString desc)
{
    _desc = desc;
}
