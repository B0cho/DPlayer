#ifndef CDATABASEMEMBER_H
#define CDATABASEMEMBER_H

#include <QList>
#include <QString>
#include <QRandomGenerator>
#include <QRegularExpression>

/// REPRESENTS DATABASE MEMBER
class CDatabaseMember
{
private:
    int _id;
public:
    CDatabaseMember(const int id);
    CDatabaseMember();
    int id() const;
    /// statics
    static QString findNewTitle(QString origin, const QStringList &usedTitles); // returns new title, unique for usedTitles
};

#endif // CDATABASEMEMBER_H
