#include "UserSet.h"

UserSet &UserSet::GetInstance()
{
    static UserSet userset;
    return userset;
}

UserSet::UserSet()
{
}
UserSet::~UserSet()
{
}