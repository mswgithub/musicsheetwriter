#ifndef IGUIDOABSTRACTOBJECT_H
#define IGUIDOABSTRACTOBJECT_H

#include    "GUIDOFactoryAdapter.h"

class IGuidoAbstractObject
{
public:
    virtual void    abstract(ARFactoryHandler& arFactory) = 0;
};

#endif // IGUIDOABSTRACTOBJECT_H
