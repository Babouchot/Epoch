#ifndef CHARACTER_OBJECT_H
#define CHARACTER_OBJECT_H

#include "LivingObject.h"

/*
*
*A Character Objet (e.g a humanoid thing that can move, player or not)
*Made of 3 pieces, a head, a body, and a bottom (wich contain the jump sensor)
*/
class CharacterObject : LivingObject {

    private :

        Object _headObject;
        Object _bodyObject;
        //the third (bottom) object is derived from the Object class

    public:

        //Joint the three parts together
        CharacterObject(vector<ObjectProperty> prop, ObjectType type);
        ~CharacterObject();
};

#endif