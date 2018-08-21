#include "cdatabasemember.h"

int CDatabaseMember::id() const
{
    return _id;
}

int CDatabaseMember::findNewId(const QList<int> usedIds)
{
    int newId;
    do
    {
        newId = QRandomGenerator::global()->bounded(1, 10000000); // generate new id
    } while(usedIds.contains(newId)); // and check if it is not already used
    return newId;
}

QString CDatabaseMember::findNewTitle(const QString origin, const QStringList usedTitles)
{
    auto titleIterator = 0;
    QString newTitle = origin;
    while(usedTitles.contains(newTitle)) newTitle = origin + " (" + QString::number(++titleIterator) + ")";
    return newTitle;
}

CDatabaseMember::CDatabaseMember(const int id) : _id(id)
{

}
