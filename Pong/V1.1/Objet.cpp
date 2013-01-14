
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

#include "Objet.h"
#include "Jeu.h"

//Variables statiques
GLuint Objet::txtballe = 0;
GLuint Objet::txtbarre = 0;

//Constructeur
Objet::Objet()
{
    type = CERCLE;
	pos.set(0,0);
	taille.set(10,10);
	vitdir.set(0.71,0.71);
    setVitesse(0.1);
	r=1.0f;g=1.0f;b=1.0;

    setDernierTps();
}

//Destructeur
Objet::~Objet()
{}

//Mettre a jour la variable derniertps
void Objet::setDernierTps()
{
	dernierTps = SDL_GetTicks();
}

//Mise en place de la texture d'une balle
void Objet::setTxtBalle(GLuint t)
{
	txtballe = t;
}

//Mise en place de la texture d'une balle
void Objet::setTxtBarre(GLuint t)
{
	txtbarre = t;
}

//Mise en place de la vitesse de l'objet
void Objet::setVitesse(double v)
{
	vitdir.normalise();
	vitdir.set(vitdir.getX()*v, vitdir.getY()*v);
}

//Mise en place de la position de l'objet
void Objet::setPos(double x1, double y1)
{
	pos.setX(x1);
	pos.setY(y1);
}

//Mise en place de la direction de l'objet
void Objet::setDirVitesse(double vx1, double vy1)
{
	vitdir.setX(vx1);
	vitdir.setY(vy1);
}

//Mise en place de la taille de l'objet
void Objet::setTaille(double w1, double h1)
{
	taille.setX(w1);
	taille.setY(h1);
}

//Recupere la position de l'objet
double Objet::getX()
{
	return pos.getX();
}

//Recupere la position de l'objet
double Objet::getY()
{
	return pos.getY();
}

//Recupere la direction de l'objet
double Objet::getVX()
{
	return vitdir.getX();
}

//Recupere la direction de l'objet
double Objet::getVY()
{
	return vitdir.getY();
}

//Recupere la taille de l'objet
double Objet::getW()
{
	return taille.getX();
}

//Recupere la taille de l'objet
double Objet::getH()
{
	return taille.getY();
}

//Recupere la couleur de l'objet
float Objet::getR()
{
	return r;
}

//Recupere la couleur de l'objet
float Objet::getG()
{
	return g;
}

//Recupere la couleur de l'objet
float Objet::getB()
{
	return b;
}

//Mise en place de la couleur de l'objet
void Objet::setCouleur(float r1, float g1, float b1)
{
	r=r1;
	g=g1;
	b=b1;
}

// Fonction d'affichage
void Objet::affiche()
{
	glEnable(GL_TEXTURE_2D);
    if(type == CERCLE)
    {
	   glBindTexture(GL_TEXTURE_2D, txtballe);
    }
    else
    {
	   glBindTexture(GL_TEXTURE_2D, txtbarre);
    }
	glColor3f(r,g,b);   
	glBegin(GL_QUADS);
    	glTexCoord2i(0,1);
    	glVertex3f(pos.getX(),pos.getY(),0);            
    	glTexCoord2i(1,1);
    	glVertex3f(pos.getX()+taille.getX(),pos.getY(),0);
    	glTexCoord2i(1,0);
    	glVertex3f(pos.getX()+taille.getX(),pos.getY()+taille.getY(),0);
    	glTexCoord2i(0,0);
    	glVertex3f(pos.getX(),pos.getY()+taille.getY(),0);      
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Fonction de mise a jour
void Objet::updatePos()
{
    //Heure actuelle
    long tps = SDL_GetTicks() - dernierTps;
    //Calcul de la nouvelle position
    double x = pos.getX() + vitdir.getX() * tps * RATIOVITESSE,
           y = pos.getY() + vitdir.getY() * tps * RATIOVITESSE,
           w = taille.getX(),
           h = taille.getY();

    //Mise à jour derniertps
    setDernierTps();

    //Mise a jour
	pos.set(x,y);

    //Verification de la position ie si on sort de l'ecran on "rebondit"
    //A gauche
    if(getType() == CERCLE)
    {
    	if(x<0)
    		{
    		pos.setX(1);
    		if(vitdir.getX()<0)
    			vitdir.setX(vitdir.getX()*(-1));
                Jeu::point(true);
    		}

        //A droite
    	if(x+w>=WIDTH)
    		{
    		pos.setX(WIDTH-w-1);
    		if(vitdir.getX()>0)
    			vitdir.setX(vitdir.getX()*(-1));
                Jeu::point(false);
    		}
    		
        //En haut
    	if(y<0)
    		{
    		pos.setY(1);
    		if(vitdir.getY()<0)
    			vitdir.setY(vitdir.getY()*(-1));
    		}
            
        //En bas 
    	if(y+h>=HEIGHT)
    		{
    		pos.setY(HEIGHT-h-1);
    		if(vitdir.getY()>0)
    			vitdir.setY(vitdir.getY()*(-1));
    		}
    }
    else
    {
        if(y+h>=HEIGHT)
        {
            setVitesse(0);
            setPos(getX(), HEIGHT - LARG_BARRE);
        }

        if(y<0)
        {
            setVitesse(0);
            setPos(getX(), 0);
        }
    }
}

//Recuperation du type
Type Objet::getType()
{
    return type;
}

//Mettre a jour le type de l'objet
void Objet::setType(Type t)
{
    type = t;
}
