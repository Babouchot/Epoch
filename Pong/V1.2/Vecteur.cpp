
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

#include "Vecteur.h"

Vecteur::Vecteur()
{
	x = 0;
	y = 0;
}

Vecteur::~Vecteur()
{
}

void Vecteur::set(double x1, double y1)
{
	x = x1;
	y = y1;
}

void Vecteur::setX(double x1)
{
	x = x1;
}

void Vecteur::setY(double y1)
{
	y = y1;
}

double Vecteur::getX()
{
	return x;
}

double Vecteur::getY()
{
	return y;
}

void Vecteur::normalise()
{
	double n = x*x + y*y;
	n = sqrt(n);
	x /= n;
	y /= n;
}

void Vecteur::setNormalise(double x1, double x2)
{
	x = x1;
	x = x2;
	normalise();
}

