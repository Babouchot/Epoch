#include "Object.h"

Object::Object(vector<ObjectProperty> prop, ObjectType type) : _properties(prop), _type(type){
    for(int i=0;i<prop.size();++i){
        _propertyOn.push_back(false);
    }
}

b2Body* Object::getB2Body(){ 
    return _b2Body;
}

int Object::getPropertyIndex(ObjectProperty prop){
    for(int i=0;i<_properties.size();++i){
        if(_properties[i]==prop){
            return i;
        }
    }
    return -1;
}

bool Object::getPropertyOn(ObjectProperty prop){
    int ind=getPropertyIndex(prop);
    if(ind !=-1){
        return _propertyOn[ind];
    }
    return false;
}

void Object::addProperty(ObjectProperty prop){
    if(getPropertyIndex(prop)==-1){
        _properties.push_back(prop);
        _propertyOn.push_back(false);
    }
}

void Object::removeProperty(ObjectProperty prop){
    int ind=getPropertyIndex(prop);
    if(ind !=-1){
        _properties.erase(_properties.begin()+ind);
        _propertyOn.erase(_propertyOn.begin()+ind);
    }
}

vector<ObjectProperty> Object::getProperties(){
    return _properties;
}

void Object::setPropertyOn(ObjectProperty prop, bool On){
    int ind=getPropertyIndex(prop);
    _propertyOn[ind]=On;
}

ObjectType Object::getType(){
    return _type;
}

void Object::render(){
    _renderer->render();
}

Object::~Object(){

}