#ifndef OBJET_H
#define OBJET_H

#include <Box2D/Box2D.h>
#include "Renderer.h"
#include "ObjectData.h"
#include <vector>

using namespace std;

class Object {

    private :

        b2Body* _b2Body;
        Renderer* _renderer;
        vector<ObjectProperty> _property;
        bool _propertyOn;
        ObjectType _type;

    public :

        Object(vector<ObjectProperty> prop, ObjectType type);
        b2Body* getB2Body();
        bool getPropertyOn();
        void addProperty(ObjectProperty prop);
        void removeProperty(ObjectProperty prop);
        vector<ObjectProperty> getProperties();
        ObjectType getType();
        virtual void render();
        ~Object();

};

#endif