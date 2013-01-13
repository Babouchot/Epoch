#ifndef H_OBJET
#define H_OBJET
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


#include "Define.h"
#include "Vecteur.h"

class Objet
{
	private:
		static GLuint txtballe,txtbarre;

		// Position, vitesse et taille de l'objet
		Vecteur pos,
			vitdir,
			taille;

		// Couleur
		float r,g,b;

        //Type
        Type type;

        //Dernier temps de mise à jour
        long dernierTps;


	public:
		//Constructeur/Destructeur
		Objet();
		~Objet();

		static void setTxtBalle(GLuint t);
		static void setTxtBarre(GLuint t);
		
		void setPos(Vecteur &pos);
		void setPos(double, double);
		void setTaille(Vecteur &t);
		void setTaille(double, double);
		void setDirVitesse(Vecteur &vit);
		void setDirVitesse(double, double);
        void setType(enum Type);

		void setVitesse(double);
		void setCouleur(float, float,float);

		//Récupération de la position
		double getX();
		double getY();
		double getVX();
		double getVY();
		double getW();
		double getH();

		float getR();
		float getG();
		float getB();
		
		//Fonction d'affichage
		void affiche();

		//Fonction de gestion  de collision
		void updatePos();

        //Recuperation du type
        Type getType();

		//Mettre a jour la variable derniertps
		void setDernierTps();
};
#endif
