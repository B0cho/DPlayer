#ifndef CDATABASEMEMBER_H
#define CDATABASEMEMBER_H

#include <QList>
#include <QString>
#include <QRandomGenerator>


class CDatabaseMember
{
private:
    int _id;
public:
    CDatabaseMember(const int id);
    CDatabaseMember();
    int id() const;
    /// statics
    static int findNewId(const QList<int> usedIds);
    static QString findNewTitle(const QString origin, const QStringList usedTitles);
};

#endif // CDATABASEMEMBER_H
