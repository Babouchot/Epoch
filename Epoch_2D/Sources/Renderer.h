#ifndef RENDERER_H
#define RENDERER_H

#include "ObjectData.h"
#include <map>
#include <string>
#include <Box2D/Box2D.h>
using namespace std;



//the top renderer class
//Meant to be abstract but this may change
//Handle the renderer/objectType matching
class Renderer
{
    protected : 

        static map<ObjectType, Renderer*> _availableRenderers;
        b2Body* _body;
        ObjectType _type;
        //Create a renderer, should be overrided in every renderer class
        virtual Renderer* createRenderer(b2Body* body)=0;

    public:

        Renderer();
        //Create and add to map
        Renderer( ObjectType type, b2Body* body=NULL );
        virtual void render() =0;
        //Get the renderer corresponding to the object type 
        //and create a new renderer
        static Renderer* getObjectRenderer( ObjectType type, b2Body* body);
        //Create a new renderer of each type
        static void initializeRenderers();
        ~Renderer();
};

#endif