#ifndef H_DEFINE
#define H_DEFINE
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


#include <SDL.h>
#include <iostream>
#include <fstream>


#ifdef __APPLE__&__MACH
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

const unsigned int MAX_BALLES=50;
const int WIDTH=800;
const int HEIGHT=600;
const int TAILLE_BALLE=20;
const int TAILLE_NEUTRE=50;
const double RATIOVITESSE = 0.1;
const int LARG_BARRE=120;
const int HAUT_BARRE=25;

const int QUADCOL=WIDTH/ (
            (TAILLE_BALLE>LARG_BARRE)?
                ((TAILLE_BALLE>HAUT_BARRE)?
                    TAILLE_BALLE:
                    HAUT_BARRE):
                ((HAUT_BARRE>LARG_BARRE)?
                    HAUT_BARRE:
                    LARG_BARRE)
           );
const int QUADLIGNE=HEIGHT/ (
            (TAILLE_BALLE>LARG_BARRE)?
                ((TAILLE_BALLE>HAUT_BARRE)?
                    TAILLE_BALLE:
                    HAUT_BARRE):
                ((HAUT_BARRE>LARG_BARRE)?
                    HAUT_BARRE:
                    LARG_BARRE)
           );

enum Type {CERCLE, BARRE};
#endif
