#include "cdatabasemember.h"

int CDatabaseMember::id() const
{
    return _id;
}

QString CDatabaseMember::findNewTitle(QString origin, const QStringList& usedTitles)
{
    auto titleIterator = 0;
    // regex to remove possible (number) at the end
    QRegularExpression regex("( \\(\\d+\\)$)"); // regex that represents number in brackets at the end of string
    origin.replace(regex, "");
    // finding (number if possible
    QString newTitle = origin;
    while(usedTitles.contains(newTitle)) newTitle = origin + " (" + QString::number(++titleIterator) + ")";
    return newTitle;
}

CDatabaseMember::CDatabaseMember(const int id) : _id(id)
{

}
