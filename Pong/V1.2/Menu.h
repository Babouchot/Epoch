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
