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
#include "Physique.h"
#include <SDL.h>

std::vector<int> Physique::tableau_zones[QUADLIGNE][QUADCOL];
int Physique::vit = 0;

//Fonction d'initialisation
bool Physique::init()
{
int i,j;
Physique::vit = 0;
for(i=0;i<QUADLIGNE;i++)
  for(j=0;j<QUADCOL;j++)
    {
    	tableau_zones[i][j].clear();
    }
return true;
}

void Physique::updateObjets(std::vector<Objet*> &objets)
{	
	int i;
	int res;
	double oldx, oldy;

	double vx,vy,newvx, newvy,n;
	double x1,x2,y1,y2;
	
    int nobjets = objets.size();

    vit++;

	for(i=0;i<nobjets;i++)
		{
		//Sauvegarde de l'ancienne position
		oldx = objets[i]->getX();
		oldy = objets[i]->getY();

		//Enlever des zones
		enleverObjetDesZones(objets, i);
		
		//Mise a jour de la position
		objets[i]->updatePos();

		//Est-ce qu'on a une collision
		res = collisionObjetZone(objets, i);

		if(res>=0)
			{
			//Si on a une collision, on remet l'ancienne position
			objets[i]->setPos(oldx,oldy);
		
			//Recupere le centre de l'objet i
			x1 = oldx + objets[i]->getW()/2;
			y1 = oldy + objets[i]->getH()/2;

			//Recupere le centre de l'objet res
			x2 = objets[res]->getX() + objets[res]->getW()/2;
			y2 = objets[res]->getY() + objets[res]->getH()/2;

			//Calcul le vecteur de direction entre les centres
			vx = x1 - x2;
			vy = y1 - y2;

			//Normalise le vecteur
			n = vx*vx + vy*vy;
			n = sqrt(n);

			vx /= n;
			vy /= n;

			//Si l'objet i est un cercle
			if(objets[i]->getType()==CERCLE)
				{
				//Mis a jour de l'objet i
				newvx = vx;
				newvy = vy;

				//Si l'objet res n'est pas un cercle, on prend la direction (newx, newy)
				//et met la vitesse à 1
				if(objets[res]->getType()!=CERCLE)
					{
					objets[i]->setDirVitesse(newvx,newvy);
                    objets[i]->setVitesse(3 + Physique::vit/1000.0);
					}
				else //Sinon on ajoute la direction (newx, newy)
					{
					objets[i]->setDirVitesse(objets[i]->getVX()+newvx, objets[i]->getVY()+newvy);
					}
				}
			
			if(objets[res]->getType()==CERCLE)
				{
				//Mis a jour de l'objet res
				newvx = (-vx);
				newvy = (-vy);

				//Si l'objet i n'est pas un cercle, on prend la direction (newx, newy)
				//et met la vitesse à 1
				if(objets[res]->getType()!=CERCLE)
					{
					objets[res]->setDirVitesse(newvx,newvy);
                    objets[res]->setVitesse(3 + Physique::vit/1000.0);
					}
				else    //Sinon on ajoute la direction (newx, newy)
					{
					objets[res]->setDirVitesse(objets[res]->getVX()+newvx,
							objets[res]->getVY()+newvy);
					}
				}
			}

		//Ajouter dans les zones
		ajouterObjetDansZones(objets, i);
		}
}

//Fonctions qui gerent les collisions
bool Physique::collisionRect(double x1, double y1, double w1, double h1,
				 double x2, double y2, double w2, double h2)
{
    if(x1+w1<x2) return false;
    if(x2+w2<x1) return false;
    if(y1+h1<y2) return false;
    if(y2+h2<y1) return false;

    return true;
}

//Collision entre deux cercles (cx1,cy1,r1) et (cx2,cy2,r2)
bool Physique::collisionCercle(double cx1, double cy1, double r1,
				 double cx2, double cy2, double r2)
{
	//Distance entre les deux centres
	double d = (cx1-cx2)*(cx1-cx2) + (cy1-cy2)*(cy1-cy2);
	d = sqrt(d);
	return (d<r1+r2);	
}

//Y-a-t-il une collision entres les objets et le rectangle (x,y,w,h) ?
bool Physique::isCollision(std::vector<Objet*> &objets, double x, double y, double w, double h)
{
	int i;
	double x1,y1,w1,h1;
    int nobjets = objets.size();

	for(i=0;i<nobjets;i++)
		{
		x1 = objets[i]->getX();
		y1 = objets[i]->getY();
		w1 = objets[i]->getW();
		h1 = objets[i]->getH();

		if(collisionRect(x1,y1,w1,h1,x,y,w,h))
			return true;
		}
	return false;
}

//Est-ce que l'objet d'indice a est en collision avec les autres ?
int Physique::collisionObjet(std::vector<Objet*> &objets, int idx)
{
	int i, nobjets = objets.size();

	double x1,x2,y1,y2;
	double w1,h1,w2,h2;
	bool cercle=false;

	double cx1=0,
	       cy1=0,
	       cr1=0, 
	       cx2=0,
	       cy2=0,
	       cr2=0;
	x1 = objets[idx]->getX();
	y1 = objets[idx]->getY();
	w1 = objets[idx]->getW();
	h1 = objets[idx]->getH();
	
	//Si c'est un cercle, on recupere son centre
	if(objets[idx]->getType()==CERCLE)
		{
		cx1 = x1+w1/2;
		cy1 = y1+h1/2;
		cr1 = w1/2;
		cercle = true;
		}
	
	//On parcourt tous les objets
	for(i=0;i<nobjets;i++)
		{
		if(idx!=i)
			{
			x2 = objets[i]->getX();
			y2 = objets[i]->getY();
			w2 = objets[i]->getW();
			h2 = objets[i]->getH();

			//Si on a deux cercles
			if( cercle && (objets[i]->getType()==CERCLE))
				{
				cx2 = x2 + w2/2;
				cy2 = y2 + h2/2;
				cr2 = w2/2;
				
				if(collisionCercle(cx1,cy1,cr1,cx2,cy2,cr2))
					return i;
				}
			else //Sinon on fait une collision entre rectangles
				{
				if(collisionRect(x1,y1,w1,h1,x2,y2,w2,h2))
					return i;
				}
			}
		}
	return -1;
}


inline void Physique::recupereCoinObjet(Objet* o, int &i, double &x, double &y)
{
	//On regarde chaque coin
	switch(i)
	{
		case 0:
			//En haut a gauche
			x = o->getX();
			y = o->getY();
			break;
		case 1:
			//En haut a droite
			x = o->getX()+o->getW();
			y = o->getY();
			break;
		case 2:
			//En bas a droite
			x = o->getX()+o->getW();
			y = o->getY()+o->getH();
			break;
		case 3:
			//En bas a gauche
			x = o->getX();
			y = o->getY()+o->getH();
			break;
		default:
			break;
	}
	
}

inline int Physique::recupereZoneColonne(double x)
{
	const int PIXPARCOL = WIDTH/QUADCOL;
	int res = (int) (x/PIXPARCOL);
	if(res>=QUADCOL)
	{
		return QUADCOL-1;
	}
	return res;
}

inline int Physique::recupereZoneLigne(double y)
{
	const int PIXPARLIGNE = WIDTH/QUADLIGNE;
	int res = (int) y/PIXPARLIGNE;
	if(res>=QUADLIGNE)
	{
		return QUADLIGNE-1;
	}
	return res;
}

void Physique::enleverObjetZone(int idx, int i, int j)
{
	int k;
	std::vector<int> &ptr = tableau_zones[i][j];
	int size = tableau_zones[i][j].size();
	
	for(k=0;k<size;k++)
	{
		if(ptr[k]==idx)
		{
			//On efface en echangeant avec le dernier 
			ptr[k] = ptr[size-1];
            ptr.pop_back();
			break;
		}
	}
}

void Physique::enleverObjetDesZones(std::vector<Objet*> &objs, int idx)
{
	double x=0,y=0;
	int oldzonei=-1, oldzonej=-1;
	int newzonei,newzonej;
	//Pour chaque coin du rectangle
	for(int i=0;i<4;i++)
	{
		//Recupere le coin
		recupereCoinObjet(objs[idx],i,x,y);
		
		//Calcul de la zone
		newzonei = recupereZoneLigne(y);
		newzonej = recupereZoneColonne(x);

		//Si c'est une autre zone
		if((newzonei!=oldzonei)||(newzonej!=oldzonej))
			{
				oldzonei = newzonei;
				oldzonej = newzonej;
				enleverObjetZone(idx,newzonei,newzonej);
			}
	}
}

void Physique::ajouterObjetZone(int idx, int i, int j)
{
	unsigned int k,
	    size = tableau_zones[i][j].size();
	std::vector<int> &ptr = tableau_zones[i][j];
	
	for(k=0;k<size;k++)
	{
		if(ptr[k]==idx)
		{
			break;
		}
	}

	if((k==size)&&(size<MAX_BALLES))
		{
		ptr.push_back(idx);
		}
}


void Physique::ajouterObjetDansZones(std::vector<Objet*> &objs, int idx)
{
	double x=0,y=0;
	int oldzonei=-1, oldzonej=-1;
	int newzonei,newzonej;
	//Pour chaque coin du rectangle
	for(int i=0;i<4;i++)
	{
		//Recupere le coin
		recupereCoinObjet(objs[idx],i,x,y);
		
		//Calcul de la zone
		newzonei = recupereZoneLigne(y);
		newzonej = recupereZoneColonne(x);

		//Si c'est une autre zone
		if((newzonei!=oldzonei)||(newzonej!=oldzonej))
			{
				oldzonei = newzonei;
				oldzonej = newzonej;
				ajouterObjetZone(idx,newzonei,newzonej);
			}
	}
}

//Est-ce que l'objet d'indice a est en collision avec les autres de sa zone ?
int Physique::collisionObjetZone(std::vector<Objet*> &objets, int idx)
{
	int i,k,l;

	double x1,x2,y1,y2;
	double w1,h1,w2,h2;
	bool cercle=false;

	double cx1=0,
	       cy1=0,
	       cr1=0, 
	       cx2=0,
	       cy2=0,
	       cr2=0;

	int nobjets_zone;
	double x=0,y=0;
	int newzonei, newzonej;
	int oldzonei=-1, oldzonej=-1;
	
	x1 = objets[idx]->getX();
	y1 = objets[idx]->getY();
	w1 = objets[idx]->getW();
	h1 = objets[idx]->getH();
	
	//Si c'est un cercle, on recupere son centre
	if(objets[idx]->getType()==CERCLE)
	{
		cx1 = x1+w1/2;
		cy1 = y1+h1/2;
		cr1 = w1/2;
		cercle = true;
	}
	
	//Pour chaque coin
	for(i=0;i<4;i++)
	{
		//Recupere le coin
		recupereCoinObjet(objets[idx],i,x,y);
		
		//Calcul de la zone
		newzonei = recupereZoneLigne(y);
		newzonej = recupereZoneColonne(x);

		//Si c'est une autre zone
		if((newzonei!=oldzonei)||(newzonej!=oldzonej))
		{
			oldzonei = newzonei;
			oldzonej = newzonej;

			//Ok on va regarder dans cette zone
			nobjets_zone = tableau_zones[newzonei][newzonej].size();
            std::vector<int> &objets_zone = tableau_zones[newzonei][newzonej];

			//On parcourt tous les objets de la zone
			for(k=0;k<nobjets_zone;k++)
			{
				l = objets_zone[k];
				
				x2 = objets[l]->getX();
				y2 = objets[l]->getY();
				w2 = objets[l]->getW();
				h2 = objets[l]->getH();

				//Si on a deux cercles
				if( cercle && (objets[l]->getType()==CERCLE))
				{
					cx2 = x2 + w2/2;
					cy2 = y2 + h2/2;
					cr2 = w2/2;

					if(collisionCercle(cx1,cy1,cr1,cx2,cy2,cr2))
						return l;
				}
				else //Sinon on fait une collision entre rectangles
				{
					if(collisionRect(x1,y1,w1,h1,x2,y2,w2,h2))
						return l;
				}
			}
		}
	}
	
	return -1;
}
