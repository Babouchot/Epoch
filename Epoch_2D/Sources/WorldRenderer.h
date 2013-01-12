#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include "Renderer.h"

//The world renderer class
//responsible for renderering the wolrd setting
class WorldRenderer : public Renderer{

    private :

        string _backgroundSpriteName;
        string _groundSpriteName;
        string _skySpriteName;

    public:

        WorldRenderer();
        WorldRenderer(string backgound, string ground, string sky);
        virtual void render();
        ~WorldRenderer();
};

#endif