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

#include "Menu.h"

using namespace std;

//Moteur du jeu 
extern Moteur moteur;

//Constructeur
Menu::Menu()
{
}

Menu::~Menu()
{
	glDeleteTextures(5,images);
}

//Fonction d'initialisation
bool Menu::init(const string &file)
{
	SDL_Surface *myimages[5];
	unsigned char *tmp;
	
	int i,j,k;
	
	//Ouverture du fichier de paramétrage
	ifstream input(file.c_str());

	//Test si le fichier s'est bien ouvert
	if(!input)
	        {
        	cerr << "Erreur dans l'ouverture du fichier " << file << endl;
	        return false;
        	}

	//Récupération du nom de l'image pour le menu
	string nom;
	input >> nom;

	//On charge l'image de fond
	myimages[0] = SDL_LoadBMP(nom.c_str());
	positions[0].x = 0;
	positions[0].y = 0;
	positions[0].w = WIDTH;
	positions[0].h = HEIGHT;
	
	//On récupère la surface de l'image "Titre"
	input >> nom;
	myimages[1] = SDL_LoadBMP(nom.c_str());
	
	//Initialisation des positionss du menu
	input >> positions[1].x >> positions[1].y;
	positions[1].w = myimages[1]->w;
	positions[1].h = myimages[1]->h;
	
	//On récupère la surface de l'image "Nouveau"
	input >> nom;
	myimages[2] = SDL_LoadBMP(nom.c_str());
	
	//Initialisation de la positions du bouton nouveau
	input >> positions[2].x >> positions[2].y;
	positions[2].w = myimages[2]->w;
	positions[2].h = myimages[2]->h;
	
	//On récupère la surface de l'image "Quitter"
	input >> nom;
	myimages[3] = SDL_LoadBMP(nom.c_str());

	//Initialisation de la positions du bouton positions[3]
	input >> positions[3].x >> positions[3].y;
	positions[3].w = myimages[3]->w;
	positions[3].h = myimages[3]->h;

	//On recupere la surface de l'image "Continuer"
	input >> nom;
	myimages[4] = SDL_LoadBMP(nom.c_str());

	//Initialisation de la positions du bouton positions[3]
	input >> positions[4].x >> positions[4].y;
	positions[4].w = myimages[4]->w;
	positions[4].h = myimages[4]->h;
	
	//Ferme le fichier	
	input.close();

	//On passe les textures SDL en textures OpenGL
	glGenTextures(5,images);
	
	for(i=0;i<5;i++)
	{
		glBindTexture(GL_TEXTURE_2D,images[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

		tmp = new unsigned char[myimages[i]->h*myimages[i]->w*4];

		for(j=0,k=0;k<myimages[i]->h*myimages[i]->w*3;)
			{
			tmp[j] = ((unsigned char*) myimages[i]->pixels)[k];
			j++;k++;
			tmp[j] = ((unsigned char*) myimages[i]->pixels)[k];
			j++;k++;
			tmp[j] = ((unsigned char*) myimages[i]->pixels)[k];
			j++;k++;
			
			if((tmp[j-1] == 0) && (tmp[j-2] == 0) && (tmp[j-3]==0))
				{
				tmp[j] = 255;
				}
			else
				{
				tmp[j] = 0;
				}
			j++;
			}
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,myimages[i]->w,myimages[i]->h,0,
				GL_BGRA,GL_UNSIGNED_BYTE,tmp);

		SDL_FreeSurface(myimages[i]);
		delete[] tmp;
	}

	//On retourne vrai
	return true;
}

//Gestion du clic
void Menu::clic(int x, int y)
{
//Est-ce qu'on est dans le bouton nouveau?
if((positions[2].x<x)&&(positions[2].x+positions[2].w>x)&&(positions[2].y<y)&&(positions[2].y+positions[2].h>y))
	{
	moteur.initJeu();
	moteur.echangeFonctions();
	}
//Est-ce qu'on est dans le bouton positions[3]?
else if((positions[3].x<x)&&(positions[3].x+positions[3].w>x)&&(positions[3].y<y)&&(positions[3].y+positions[3].h>y))
	{
	moteur.fin();
	}
}

//Gestion du clavier
bool Menu::clavier(unsigned char k)
{
	switch(k)
	{
		case 'c':
			moteur.echangeFonctions();
			return true;
		case 'n':
			moteur.initJeu();
			moteur.echangeFonctions();
			return true;
		default:
			return false;
	}
}

//Fonction d'affichage
void Menu::affiche()
{
	int i;
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glAlphaFunc(GL_LESS,10);
	glEnable(GL_ALPHA_TEST);

	//On dessine l'image de fond

	glColor3f(1.0,1.0,1.0);
	for(i=0;i<4;i++)
		{
		glBindTexture(GL_TEXTURE_2D, images[i]);
		glBegin(GL_QUADS);
			glTexCoord2i(0,0);glVertex2i(positions[i].x,positions[i].y); 
			glTexCoord2i(1,0);glVertex2i(positions[i].x+positions[i].w,positions[i].y);
			glTexCoord2i(1,1);glVertex2i(positions[i].x+positions[i].w,positions[i].y+positions[i].h);
			glTexCoord2i(0,1);glVertex2i(positions[i].x,positions[i].y+positions[i].h);
		glEnd();
		}
	
	//Pour le dernier bouton, cela dépend si le jeu est en cours
	//Remarque i vaut 4, donc c'est bien
	if(moteur.jeuEnCours())
	{
		glBindTexture(GL_TEXTURE_2D, images[i]);
		glBegin(GL_QUADS);
			glTexCoord2i(0,0);glVertex2i(positions[i].x,positions[i].y); 
			glTexCoord2i(1,0);glVertex2i(positions[i].x+positions[i].w,positions[i].y);
			glTexCoord2i(1,1);glVertex2i(positions[i].x+positions[i].w,positions[i].y+positions[i].h);
			glTexCoord2i(0,1);glVertex2i(positions[i].x,positions[i].y+positions[i].h);
		glEnd();
	}
	
	glDisable(GL_TEXTURE_2D);
}
