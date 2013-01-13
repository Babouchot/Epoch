#include "Renderer.h"
#include "BasicRenderer.h"

map<ObjectType, Renderer*> Renderer::_availableRenderers;

Renderer::Renderer( ){

}

Renderer::Renderer( ObjectType type, b2Body* body ) : _body(body) {
    _availableRenderers[type]=this;
    _type=type;
}

Renderer* Renderer::getObjectRenderer( ObjectType type, b2Body* body ){
    return _availableRenderers[type]->createRenderer(body);
}

void Renderer::initializeRenderers(){
    new BasicRenderer(MIDDLE_AGE_BLOCK);
    new BasicRenderer(MIDDLE_AGE_THIN_WALL);
    new BasicRenderer(MIDDLE_AGE_THIN_PLATFORM);
}

Renderer::~Renderer(){

}