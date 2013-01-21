#ifndef H_RAND
#define H_RAND
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


#include <stdlib.h>
#include <time.h>
#include "Define.h"

class Rand
{
	private:
		static int cnt;

		Rand();		
		~Rand();
		
		static void checkSeed();

	public:	
		static void newSeed(int s);
		static int randi();
		static int randi(int max);
		static double randf();
		static double randf(double a, double b);
};

#endif
