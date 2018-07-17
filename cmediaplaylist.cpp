#include "cmediaplaylist.h"

CMediaPlaylist::CMediaPlaylist(const int id, const QString title, const QString description) : CDatabaseMember(id), title(title), description(description)
{
    //
}

void CMediaPlaylist::addFragment(const CMediaFragment* fragment, const int position)
{
    _list.insert(position, fragment);
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
