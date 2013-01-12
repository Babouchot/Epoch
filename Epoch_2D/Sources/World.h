#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Object.h"
#include "LivingObject.h"
#include "DeadObject.h"
#include "WorldRenderer.h"

using namespace std;

//Abstract worlds class, shouldn't be use as is
class World
{
    private : 

        b2World* _world;
        WorldRenderer _worldRenderer;
        vector<Object *> _objectVector;

    public:
        //the seeting of the world should take place in the constructor
        World();
        //step to the next world state
        void step();
        //go through the _objectVector vector and render each object
        //call the WorldRenderer
        void render();
        ~World();
};

#endif