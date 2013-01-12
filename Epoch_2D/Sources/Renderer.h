#ifndef RENDERER_H
#define RENDERER_H

#include "ObjectData.h"
#include <map>
#include <string>

using namespace std;



//the top renderer class
//Meant to be abstract but this may change
//Handle the renderer/objectType matching
class Renderer
{
    private : 

        map<ObjectType, Renderer*> _availableRenderers;

    public:

        Renderer();
        //Create and add to map
        Renderer( ObjectType type );
        virtual void render() =0;
        //Get the renderer corresponding to the object type
        static Renderer* getRenderer( ObjectType type );
        //Create a new renderer of each type
        static void initializeRenderers();
        ~Renderer();
};

#endif