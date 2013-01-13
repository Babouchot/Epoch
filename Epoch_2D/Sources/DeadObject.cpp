#include "DeadObject.h"

DeadObject::DeadObject(vector<ObjectProperty> prop, ObjectType type) : Object(prop, type){

}

DeadObject::DeadObject(LivingObject obj) : Object(obj.getProperties(), obj.getType()){

}

DeadObject::~DeadObject(){

}