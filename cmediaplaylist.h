#ifndef CMEDIAPLAYLIST_H
#define CMEDIAPLAYLIST_H

#include <QMediaPlaylist>
#include <QString>
#include <QMap>
#include "cmediafragment.h"

typedef QMap<int, const CMediaFragment*> CMFragmentsMap;
class CMediaPlaylist : public CDatabaseMember
{
public:
    CMediaPlaylist(const int id, const QString title, const QString description);
    void addFragment(const CMediaFragment* fragment, const int position);
    // return size of playlist
    int size() const;
    QString title;
    QString description;
    int getPosition(const CMediaFragment* frag) const;
private:
    CMFragmentsMap _list;

};

#endif // CMEDIAPLAYLIST_H
