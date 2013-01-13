#ifndef BASIC_RENDERER_H
#define BASIC_RENDERER_H

#include "Renderer.h"

//Basic renderer class, use to render simple object like bloc or platform
//Simply apply a sprite on top of the object
//Streach the sprite to fit ? duplicate the sprite ?
class BasicRenderer : public Renderer {

    private :
        //used to retrieve/load the sprite
        //supposed to be the ObjectTye string, if a such thing is possible
        string _spriteName;

    public:

        BasicRenderer(ObjectType type, string sprite);
        virtual void render();
        ~BasicRenderer();
};

#endif