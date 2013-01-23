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

#include "Moteur.h"

//Constructeur
Moteur::Moteur()
{
	dans_menu = true;
    jeu = new Jeu();
    menu = new Menu();
}

//Destructeur
Moteur::~Moteur()
{
    delete jeu;
    delete menu;
}


void Moteur::gereScene()
{
	// Effacer le tampon des couleurs
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if(!dans_menu)
		dessineJeu();
	else
		dessineMenu();

	SDL_GL_SwapBuffers();
}

void Moteur::dessineMenu()
{
    menu->affiche();
}

void Moteur::dessineJeu()
{
	jeu->affiche();
	jeu->gereSceneServeur();
    jeu->afficheScore();
    if(jeu->aff)
      jeu->afficheStart();
}

bool Moteur::init()
{
	return jeu->init() && menu->init("data/menu.txt");
}

void Moteur::clavier(unsigned int k)
{
    int gere=false;
    if(dans_menu)
    {
        gere = menu->clavier(k);
    }
    else 
    {
        gere = jeu->clavier(k);
    }

    //Si la touche n'est pas geree
    if(!gere) {
        switch(k)
        {
            //On veut quitter 
            case 'q':
                fin();
                break;
            default:
                break;
        }
    }
}

void Moteur::fin()
{
    est_fini = true;
}

bool Moteur::estFini()
{
    return est_fini;
}

//Echange entre menu et jeu
void Moteur::echangeFonctions()
{
    dans_menu = !dans_menu;
    if(!dans_menu) 
    {
        jeu->toucheObjets();
    }
}

//Recommence la partie
void Moteur::initJeu()
{
    jeu->recommence();
}

//Est-ce que le jeu est en cours
bool Moteur::jeuEnCours()
{
    return jeu->enCours();
}

void Moteur::reInit()
{
    jeu->reInit();
}

