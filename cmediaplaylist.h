#ifndef CMEDIAPLAYLIST_H
#define CMEDIAPLAYLIST_H

#include <QMediaPlaylist>
#include <QString>
#include <QMap>
#include "cmediafragment.h"

/// REPRESENTS PLAYLIST
typedef QMap<int, const CMediaFragment*> CMFragmentsMap;
class CMediaPlaylist : public CDatabaseMember
{
public:
    /// ctor/dtors
    CMediaPlaylist(const int id, const QString title, const QString description);

    /// methods
    void addFragment(const CMediaFragment* fragment, int position); // adds given fragment to given position
    void addFragment(const QList<CMediaFragment*>& fragment); // appending fragment
    int size() const; // return size of playlist
    int getPosition(const CMediaFragment* frag) const; // returns positions of given fragments
    CMFragmentsMap* getList(); // returns map of fragments
    const CMFragmentsMap getList() const;

    /// members
    QString title;
    QString description;

private:
    /// members
    CMFragmentsMap _list; // map of fragments

};

#endif // CMEDIAPLAYLIST_H
