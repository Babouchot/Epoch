#ifndef OBJET_H
#define OBJET_H

#include <Box2D/Box2D.h>
#include "Renderer.h"
#include "ObjectData.h"
#include <vector>

using namespace std;

class Object {

    protected :

        b2Body* _b2Body;
        Renderer* _renderer;
        vector<ObjectProperty> _properties;
        vector<bool> _propertyOn;
        ObjectType _type;

    public :

        Object(vector<ObjectProperty> prop, ObjectType type);
        b2Body* getB2Body();
        int getPropertyIndex(ObjectProperty prop);
        bool getPropertyOn(ObjectProperty prop);
        void addProperty(ObjectProperty prop);
        void removeProperty(ObjectProperty prop);
        vector<ObjectProperty> getProperties();
        void setPropertyOn(ObjectProperty prop, bool On);
        ObjectType getType();
        virtual void render();
        ~Object();

};

#endif