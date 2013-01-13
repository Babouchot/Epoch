#include "BasicRenderer.h"
#include <sstream>


BasicRenderer::BasicRenderer(ObjectType type, b2Body* body )  : 
                        Renderer(type, body) {
    ostringstream os;
    os<<type;
    _spriteName=os.str();
    delete &os;
                            //Load sprite with _spriteName
}

void BasicRenderer::render(){
    //display sprite
}

BasicRenderer::~BasicRenderer(){

}

Renderer* BasicRenderer::createRenderer(b2Body* body){
    return new BasicRenderer(_type, body);
}
