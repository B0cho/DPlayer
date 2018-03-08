#include "cdatabasemember.h"

int CDatabaseMember::id() const
{
    return _id;
}

CDatabaseMember::CDatabaseMember(const int id) : _id(id)
{

}
