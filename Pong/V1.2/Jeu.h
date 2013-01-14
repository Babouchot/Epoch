#ifndef H_JEU
#define H_JEU


//On va definir une classe Jeu
class Jeu;

/*
Un pong en SDL/OpenGL
A pong in SDL/OpenGL
Copyright (C) 2006 BEYLER Jean Christophe

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

#ifdef __APPLE__&__MACH__
#include <OpenGL/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <vector>

#include "Objet.h"
#include "Rand.h"
#include "Moteur.h"
#include "Physique.h"

class Jeu
{
	private:
	static int j1;
	static int j2;


        //Les objets
		std::vector<Objet*> objets;

        Objet* balle;
        Objet* barre;
        Objet* ennemi;
        //Pour la creation des balles
		unsigned int last;
	
        //Pour savoir si la partie est en cours
        bool jeuEnCours;

		//Initialisation des objets et des textures
		bool initObjets();
		bool initTextures();

	public:
		//Constructeur et Destructeur
		Jeu();
		~Jeu();

		//Initialisation de la classe
		bool init();

		//Ajouter un objet dans le jeu, retourne l'indice de l'objet ajoute
		int addObjet(Objet* o);
		//Retourner le nombre d'objets dans le jeu
		unsigned int getNObjets();

		//Afficher le jeu
		void affiche();
		void affObjets();

		//Gere la scene (mise a jour)
		void gereSceneServeur();

        //Gestion du clavier
        bool clavier(unsigned int, int);

        //Recommence la partie
        void recommence();

        //Est-ce que le jeu est en cours
        bool enCours();

        //Mis a jour du temps pour les objets 
        void toucheObjets();

        static void point(bool);
	
       
};
#endif
