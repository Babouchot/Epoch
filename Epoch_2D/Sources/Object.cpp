#include "Object.h"

Object::Object(vector<ObjectProperty> prop, ObjectType type){

}

b2Body* Object::getB2Body(){ 
    return _b2Body;
}

bool Object::getPropertyOn(){
    return _propertyOn;
}

void Object::addProperty(ObjectProperty prop){

}

void Object::removeProperty(ObjectProperty prop){
    
}

vector<ObjectProperty> Object::getProperties(){

}

ObjectType Object::getType(){

}

void Object::render(){

}

Object::~Object(){

}