#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <string>

using namespace std;

//The world renderer class
//responsible for renderering the wolrd setting
class WorldRenderer {

    private :

        string _backgroundSpriteName;
        string _groundSpriteName;
        string _skySpriteName;

    public:

        WorldRenderer();
        WorldRenderer(string backgound, string ground, string sky);
        void render();
        ~WorldRenderer();
};

#endif