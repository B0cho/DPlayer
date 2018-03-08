#ifndef CDATABASEMEMBER_H
#define CDATABASEMEMBER_H


class CDatabaseMember
{
private:
    int _id;
public:
    CDatabaseMember(const int id);
    int id() const;
};

#endif // CDATABASEMEMBER_H
