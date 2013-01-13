#ifndef H_VECTEUR
#define H_VECTEUR
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


#include <cmath>

class Vecteur
{
	private:
		double x, y;
		
	public:
		Vecteur();
		~Vecteur();

		void set(double, double);
		void setX(double x);
		void setY(double y);
		void normalise();

		void setNormalise(double, double);

		double getX();
		double getY();
};
#endif

