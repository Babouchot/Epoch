/* piece.cpp */
#include "piece.h"

Piece::Piece()
{
    // Rien à faire
}

Piece::Piece(int k, int o)
{
	kind = k;
	orientation = o;
}

Piece::Piece(const Piece &p)
{
	kind = p.kind;
	orientation = p.orientation;
	color = p.color;
	posX = p.posX;
	posY = p.posY;
}

void Piece::setKind(int k)
{
	kind = k;
}

void Piece::setOrientation(int o)
{
	orientation = o;
}

int Piece::getKind()
{
	return kind;
}

int Piece::getOrientation()
{
	return orientation;
}

void Piece::setColor(int c)
{
	color = c;
}

int Piece::getColor()
{
	return color;
}

void Piece::setPosX(int x)
{
	posX = x;
}

int Piece::getPosX()
{
	return posX;
}

void Piece::setPosY(int y)
{
	posY = y;
}

int Piece::getPosY()
{
	return posY;
}
