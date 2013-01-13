
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

#include "Rand.h"

//Mis en place du compteur
int Rand::cnt=0;

//Vérification du compteur
void Rand::checkSeed()
{
	//Si on a trop appelé le générateur de nombres, on remet le tout à zéro
	if((!cnt)||(cnt>10000))
		{
		srand(time(NULL));
		cnt=1;
		}
}

//Générer une nouvelle séquence...
void Rand::newSeed(int s)
{
	srand(s);
}

//Générer un nombre aléatoire
int Rand::randi()
{
	checkSeed();
	return rand();
}

//Générer un nombre aléatoire entre [0,max-1]
int Rand::randi(int max)
{
	checkSeed();
	if(max)
		{
		return rand()%max;
		}
	return 0;
}

//Générer un nombre aléatoire entre [0,1]
double Rand::randf()
{
	checkSeed();
	return rand()/((double) RAND_MAX);
}

//Générer un nombre aléatoire entre [a,b]
double Rand::randf(double a, double b)
{
	checkSeed();
	if(a>b)
		return randf(b,a);
	return (randf()*(b-a)+a);
}
