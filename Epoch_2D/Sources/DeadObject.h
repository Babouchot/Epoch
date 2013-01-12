#ifndef DEAD_OBJECT_H
#define DEAD_OBJECT_H

#include "Object.h"
#include "LivingObject.h"

class DeadObject : public Object {

    private : 



    public:

        DeadObject(vector<ObjectProperty> prop, ObjectType type);
        DeadObject(LivingObject obj);
        ~DeadObject();
};

#endif