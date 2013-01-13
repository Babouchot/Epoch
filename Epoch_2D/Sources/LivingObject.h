#ifndef LIVING_OBJECT_H
#define LIVING_OBJECT_H

#include "Object.h"

enum ActionInput{
    LEFT,
    RIGHT,
    JUMP,
    ACTION,
    HIT,
};

class LivingObject : public Object {

    protected :

        Renderer* _movementRenderer;
        Renderer* _actionRenderer;
        Renderer* _hitRenderer;
        Renderer* _jumpRenderer;
        bool _isFacingLeft;

    public:

        LivingObject(vector<ObjectProperty> prop, ObjectType type);
        //Make the living object perform an action
        void act(ActionInput intput);
        ~LivingObject();
};

#endif