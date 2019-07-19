#include "cmediafragment.h"

CMediaFragment::CMediaFragment(const int id, const QString file_path, const quint64 created, const qint64 file_size)
    : CDatabaseMember(id),
      _file(QFileInfo(file_path)),
      _duration(0)
{
    if(!_file.isFile()) throw NO_FILE;
    if(!_file.exists() || _file.size() != file_size || _file.created().toSecsSinceEpoch() != created) throw INVALID;
    if(!_file.isReadable()) throw NO_READABLE;
}

CMediaFragment::CMediaFragment(const int id, const QFileInfo file_info)
    : CDatabaseMember(id),
      _file(file_info),
      _duration(0)
{

}

CMediaFragment::CMediaFragment(const int id, const CMediaFragment &toBeCopied)
    :  CDatabaseMember(id),
      _file(toBeCopied.file()),
      _title(toBeCopied.title()),
      _start(toBeCopied.start()),
      _end(toBeCopied.end()),
      _desc(toBeCopied.file().baseName()),
      _duration(toBeCopied.getDuration())
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

bool CMediaFragment::setStart(const qint16 start)
{
    if(startScope <= start && start <= endScope)
    {
        _start = start;
        return true;
    } else return false;
}

qint16 CMediaFragment::end() const
{
    return _end;
}

bool CMediaFragment::setEnd(const qint16 end)
{
    if(startScope <= end && end <= endScope)
    {
        _end = end;
        return true;
    } else return false;
}

QString CMediaFragment::desc() const
{
    return _desc;
}

void CMediaFragment::setDesc(const QString desc)
{
    _desc = desc;
}

QTime CMediaFragment::getStart() const
{
    const auto msc = _duration * startScope / 1000;
    QTime startTime(0, 0, 0);
    return startTime.addMSecs(msc);
}

QTime CMediaFragment::getEnd() const
{
    const auto msc = _duration * endScope / 1000;
    QTime endTime(0, 0, 0);
    return endTime.addMSecs(msc);
}

qint64 CMediaFragment::getDuration() const
{
    return _duration;
}

bool CMediaFragment::setDuration(const qint64 &duration)
{
    if(_duration || duration < 0) return false;
    _duration = duration;
    return true;
}

