/*
Un pong en SDL/OpenGL
A pong in SDL/OpenGL
Thanks to BEYLER Jean Christophe
Modifications made by MAUGARD Matthieu

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "Main.h"

using namespace std;

//Variable globale
Moteur moteur;

int main(int argc, char **argv)
{
    SDL_Event event;
    SDL_Surface *screen;
    int fps, last, now;
    FPSmanager manager;

    #ifdef __linux
    glutInit(&argc, argv);
    #endif
    //Initialisation
    if(SDL_Init(SDL_INIT_VIDEO)!=0) {
        cerr << "Probleme pour initialiser SDL: " << SDL_GetError() << endl;
        return EXIT_FAILURE;
    }

    //Mettre un titre à la fenêtre
    SDL_WM_SetCaption("Pong Version 1.2", NULL);

    //Ouvrir une fenetre
    screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_OPENGL);
    if(screen==NULL)
        return EXIT_FAILURE;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    // Mettre le systeme de coordonnees a zero avant de modifier
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //Mettre la bonne perspective
    glOrtho(0,WIDTH,HEIGHT,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();                                            

    //Initialisation du moteur
    if(moteur.init()==false)
        return EXIT_FAILURE;

    //Initialisation 
    SDL_initFramerate(&manager);

    //Mettre le nombre d'images par secondes souhaite
    SDL_setFramerate(&manager,60);

    //Pour le calcul des FPS
    last = SDL_GetTicks()/1000;
    fps = 0;

    //Boucle generale
    while(moteur.estFini()==false)
    {
        //Traiter les evenements
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    moteur.fin();
                    break;
                case SDL_KEYDOWN:
            		moteur.clavier(event.key.keysym.sym);
                    break;
		  break;
                default:
                    break;
            }
        }

        // Demander au moteur de dessiner la scene
        moteur.gereScene();
        SDL_framerateDelay(&manager);

        //Pour le calcul de fps
        fps++;
        now = SDL_GetTicks()/1000;
    }

    SDL_Quit();

    (void) argc;
    (void) argv;
    return EXIT_SUCCESS;
}
