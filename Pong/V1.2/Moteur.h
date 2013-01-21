#ifndef H_MOTEUR
#define H_MOTEUR

//On va definir une classe Moteur
class Moteur;

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


#include "Define.h"
#include "Jeu.h"
#include "Menu.h"

class Moteur
{
  private:
    // Le jeu
    Jeu *jeu; 
    Menu *menu;

    // Sommes-nous dans le menu ?
    bool dans_menu;

    // Dessiner le jeu
    void dessineJeu();
    // Dessiner le menu
    void dessineMenu();

    //Est-ce que la partie est terminee 
    bool est_fini;
    
  public:
    // Le constructeur et destructeur
    Moteur();
    ~Moteur();

    // Gerer la scene (affichage + mise a jour)
    void gereScene();

    // Fonction d'initialisation
    bool init();

    //Gestion du clavier
    void clavier(unsigned int k);

    //Est-ce que la partie est finie ?
    bool estFini();
    //Terminer la partie
    void fin();

    //Echange entre menu et jeu
    void echangeFonctions();

    //Initialise la partie
    void initJeu();

    //Est-ce que le jeu est en cours
    bool jeuEnCours();

    void reInit();
};

#endif
