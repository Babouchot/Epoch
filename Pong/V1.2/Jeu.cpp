
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

#include "Jeu.h"
#include <sstream>
#include <string>
#ifdef __APPLE__&__MACH__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//Variable globale
extern Moteur moteur;

int Jeu::j1 = 0;
int Jeu::j2 = 0;

//Constructeur
Jeu::Jeu()
{
    last = 0;
    jeuEnCours = false;
}	

//Destructeur
Jeu::~Jeu()
{
}

bool Jeu::initTextures()
{
    int i,j,k,cnt,pcnt;
    GLuint txtsurf;

    unsigned char *pixels, *tmppixels;

    SDL_Surface *tmpsurf, *surf;

    for(k=0;k<2;k++)
    {

        if(k==0) 
        {
            //On charge l'image "data/balle.bmp"
            tmpsurf = SDL_LoadBMP("data/balle.bmp");
        }
        else
        {
            //On charge l'image "data/barre.bmp"
            tmpsurf = SDL_LoadBMP("data/barre.bmp");
        }

        if(tmpsurf==NULL)
        {
            std::cerr << "Erreur dans le chargement de l'image de la surface : " << SDL_GetError() << std::endl; 
            return false;
        }

        //On cree une surface RGBA de meme dimension
        surf = SDL_CreateRGBSurface(SDL_SWSURFACE, tmpsurf->w, tmpsurf->h, 32,
                0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        if(surf==NULL)
        {
            std::cerr << "Erreur dans la creation de l'image de la surface " << k << std::endl;
            return false;
        }


        pixels = (unsigned char*) surf->pixels;
        tmppixels = (unsigned char*) tmpsurf->pixels;

        pcnt = 0;
        cnt = 0;

        //Pour chaque pixel
        for(i=0;i<tmpsurf->w;i++)
        {
            for(j=0;j<tmpsurf->h;j++)
            {
                //On copie la couleur
                pixels[pcnt] = tmppixels[cnt];
                pixels[pcnt+1] = tmppixels[cnt+1];
                pixels[pcnt+2] = tmppixels[cnt+2];

                //La couleur magenta sera transparente
                if((tmppixels[cnt]==255)&&(tmppixels[cnt+1]==0)&&(tmppixels[cnt+2]==255))
                {
                    pixels[pcnt+3] = 255;
                }
                else
                {
                    pixels[pcnt+3] = 0;
                }

                cnt += 3;
                pcnt += 4;
            }
        }

        //On passe la texture a OpenGL

        //Generation d'un indice de texture
        glGenTextures(1,&txtsurf);

        //On choisit la texture et on definit un simple filtre pour la texture    
        glBindTexture(GL_TEXTURE_2D,txtsurf);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

        //Chargement de l'image
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surf->w,surf->h,0,
                GL_RGBA,GL_UNSIGNED_BYTE,surf->pixels);

        //Liberation des surfaces    
        SDL_FreeSurface(surf);
        SDL_FreeSurface(tmpsurf);

        if(k==0)
        {
            //On passe la texture aux objets de type balle
            Objet::setTxtBalle(txtsurf);
        }
        else
        {
            //On passe la texture aux objets de type barre
            Objet::setTxtBarre(txtsurf);
        }
    }
    return true;
}

//Initialisation du jeu
bool Jeu::init()
{
    j1 = 0;
    j2 = 0;
    pouet = true;
    return initObjets() && initTextures() && Physique::init();
}

bool Jeu::initObjets()
{
    objets.clear();
    
    return true;
}

//Recommence la partie
void Jeu::recommence()
{
    Objet* o = new Objet();
    Objet* pad = new Objet();
    Objet* b = new Objet();
    pouet = true;

    j1 = j2 = 0;
   
    objets.clear();
    Physique::init();
    jeuEnCours = true;

    //Premiere barre
    pad->setType(BARRE);
    pad->setVitesse(0);
    pad->setCouleur(0.0,1.0,0.0);
    pad->setTaille(HAUT_BARRE,LARG_BARRE);
    pad->setPos(HAUT_BARRE/2, HEIGHT/2 - LARG_BARRE/2);
    barre = pad;
    addObjet(pad);

    //2eme barre
    o->setType(BARRE);
    o->setVitesse(0);
    o->setCouleur(2.0,0.12,0.32);
    o->setTaille(HAUT_BARRE,LARG_BARRE);
    o->setPos(WIDTH - 3*HAUT_BARRE/2, HEIGHT/2 - LARG_BARRE/2);
    ennemi = o;
    addObjet(o);

    // Definit la position de la balle
    b->setType(CERCLE);
    b->setVitesse(0);
    b->setTaille(TAILLE_BALLE, TAILLE_BALLE);
    b->setPos(WIDTH/2 - TAILLE_BALLE/2, HEIGHT/2 - TAILLE_BALLE/2);
    b->setCouleur(1.0, 1.0, 1.0);
    balle = b;
    // On ajoute l'objet
    addObjet(b);
}

int Jeu::addObjet(Objet* o)
{
    if(getNObjets()<MAX_BALLES)
    {
        objets.push_back(o);
        return getNObjets()-1;
    }
    return -1;
}

void Jeu::affiche()
{
    affObjets();
}

void Jeu::affObjets()
{
    unsigned int i;

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_LEQUAL,0.1f);
    for(i=0;i<getNObjets();i++)
    {
        objets[i]->affiche();
    }
    glDisable(GL_ALPHA_TEST);
}

inline unsigned int Jeu::getNObjets()
{
    return objets.size();
}

void Jeu::gereSceneServeur()
{
	//Mis a jour des objets
    if(balle->getVY() > 0)
    {
        ennemi->setDirVitesse(0.0, 1.0);
        ennemi->setVitesse(2.0);
    }
    else
    {
        if(balle->getVY() == 0)
        {
            ennemi->setDirVitesse(0.0, 0.001);
            ennemi->setVitesse(0);
        }
        else
        {
            ennemi->setDirVitesse(0.0, -1.0);
            ennemi->setVitesse(2.0);
        }
    }

    Physique::updateObjets(objets);
}

//Gestion du clavier
bool Jeu::clavier(unsigned int k, int i)
{
    switch(k)
    {
        //On veut voir le menu
        case SDLK_q:
            moteur.echangeFonctions();
            return true;
        case SDLK_s:
            balle->setDirVitesse(1.0,0.0);
            balle->setVitesse(3.0);
            if(pouet)
            {
                pouet = !pouet;
            }
            return true;
        case SDLK_DOWN:
            if(i)
            {
                if(barre->getY() + LARG_BARRE <= HEIGHT)
                {
                    barre->setDirVitesse(0.0, 1.0);
                    barre->setVitesse(2.0);
                }
                else
                {
                    barre->setPos(barre->getX(), HEIGHT - LARG_BARRE);
                }
            }
            else
            {
                barre->setVitesse(0);
            }
            return true;
        case SDLK_UP:
        if(i)
            {
                if(barre->getY() + LARG_BARRE <= HEIGHT)
                {
                    barre->setDirVitesse(0.0, -1.0);
                    barre->setVitesse(2.0);
                }
                else
                {
                    barre->setPos(barre->getX(), HEIGHT - LARG_BARRE);
                }
            }
            else
            {
                barre->setVitesse(0);
            }
            return true;
        default:
            return false;
    }
}

//Est-ce que le jeu est en cours ?
bool Jeu::enCours()
{
    return jeuEnCours;
}

void Jeu::toucheObjets()
{
    unsigned int i,
        nobjets = objets.size();
            
    for(i=0;i<nobjets;i++)
    {
        objets[i]->setDernierTps();
    }
}

void Jeu::point(bool i)
{
  if (i) {
    if (j1 == 9) {
      std::cout << "Bravo vous avez gagné ! :)" << std::endl;
      moteur.initJeu();
    }
    else 
      j1++;
  }
  else {
    if (j2 == 9) {
      std::cout << "Mince vous avez perdu ! :(" << std::endl;
      moteur.initJeu();
    }
    else
      j2++;
  }
}

void Jeu::afficheScore()
{
    std::stringstream ss;
    ss << Jeu::j1 << " : " << Jeu::j2;
    std::string score = ss.str();
    glPushMatrix();
 
    glLoadIdentity();   
 
    glRasterPos3f(WIDTH/2 - 20, 39,-1.);
 
    for (unsigned int i=0;i<score.size();i++) 
     glutBitmapCharacter(GLUT_BITMAP_8_BY_13,score[i]);
 
    glPopMatrix();
}

void Jeu::afficheStart()
{
  std::string debut_partie = "Appuyez sur S pour commencer";
    glPushMatrix();
 
    glLoadIdentity();   
 
    glRasterPos3f(WIDTH/2 - 112, 117,-1.);
 
    for (unsigned int i=0;i<debut_partie.size();i++) 
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13,debut_partie[i]);
 
    glPopMatrix();

}
