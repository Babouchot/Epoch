#ifndef LIVING_OBJECT_H
#define LIVING_OBJECT_H

#include "Object.h"

enum ActionInput{
    LEFT,
    RIGHT,
    JUMP,
    ACTION,
};

class LivingObject : public Object {

    private :



    public:

        LivingObject(vector<ObjectProperty> prop, ObjectType type);
        //Make the living object perform an action
        void act(ActionInput intput);
        ~LivingObject();
};

#endif