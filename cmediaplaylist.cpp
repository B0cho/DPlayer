#include "cmediaplaylist.h"

CMediaPlaylist::CMediaPlaylist(const int id, const QString title, const QString description) : CDatabaseMember(id), title(title), description(description)
{
    //
}

void CMediaPlaylist::addFragment(const CMediaFragment* fragment,  int position)
{
    if(_list.contains(position))
    {
        // if already exists take the last and add one
        position = _list.lastKey() + 1;
    }
    _list.insert(position, fragment);
}

void CMediaPlaylist::addFragment(const QList<CMediaFragment *> &fragment)
{
    int pos = (_list.empty()) ? 1 : _list.lastKey() + 1;
    foreach (const auto i, fragment) {
        addFragment(i, pos++);
    }
}

int CMediaPlaylist::size() const
{
    return _list.size();
}

int CMediaPlaylist::getPosition(const CMediaFragment *frag) const
{
    return _list.key(frag, 0);
}

CMFragmentsMap* CMediaPlaylist::getList()
{
    return &_list;
}
