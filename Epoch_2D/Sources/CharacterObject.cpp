#include "CharacterObject.h"

CharacterObject::CharacterObject(vector<ObjectProperty> prop, ObjectType type) : 
                                LivingObject(prop, type),
                                _headObject(prop, type),
                                _bodyObject(prop, type) {

}

CharacterObject::~CharacterObject(){

}