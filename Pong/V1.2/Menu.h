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

#ifndef H_MENU
#define H_MENU

class Menu;

#include "Define.h"
#include "Moteur.h"


class Menu
{
	private:
		//Surfaces pour le menu
		GLuint images[5];

		SDL_Rect positions[5];
		
		//Fonctions privées :
		Menu(const Menu&);            //Constructeur par copie
		Menu& operator=(const Menu&); //Surcharge de l'opérateur =
	public:
		//Constructeur
		Menu();
		//Destructeur
		~Menu();

		//Fonction d'initialisation
		bool init(const std::string &file);
		
		//Gestion du clic
		void clic(int x, int y);

		//Gestion du clavier
		bool clavier(unsigned char k);
		
		//Fonction d'affichage
		void affiche();
};

#endif
