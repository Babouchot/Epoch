/* board.cpp */
#include "board.h"

void Board::flood(int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE])
{
	if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
		return;

	visited[px][py] = true;
	area[j][i] = value;

	flood(i, j - 1, px, py - 1, k, o, value, visited);
	flood(i + 1, j, px + 1, py, k, o, value, visited);
	flood(i, j + 1, px, py + 1, k, o, value, visited);
	flood(i - 1, j, px - 1, py, k, o, value, visited);
}

void Board::flood(int i, int j, int px, int py, int k, int o, bool &flag, bool visited[][SIZE])
{
	if(px < 0 || px >= SIZE || py < 0 || py >= SIZE || visited[px][py] || PIECES[k][o][px][py] == FREE)
		return;

	visited[px][py] = true;

	if(i < 0 || i >= BOARD_HEIGHT || j < 0 || j >= BOARD_WIDTH || area[j][i] != FREE )
	{
		flag = false;
		return;
	}

	flood(i, j - 1, px, py - 1, k, o, flag, visited);
	flood(i + 1, j, px + 1, py, k, o, flag, visited);
	flood(i, j + 1, px, py + 1, k, o, flag, visited);
	flood(i - 1, j, px - 1, py, k, o, flag, visited);
}

void Board::floodFill(int i, int j, int px, int py, int k, int o, int value)
{
	bool visited[SIZE][SIZE];

	for(int l = 0; l < SIZE; ++l)
		for(int m = 0; m < SIZE; ++m)
			visited[l][m] = false;

	flood(i, j, px, py, k, o, value, visited);
}

Board::Board()
{
	for(int i = 0; i < BOARD_WIDTH; ++i)
		for(int j = 0; j < BOARD_HEIGHT; ++j)
			area[i][j] = FREE;
}

void Board::drawPiece(Piece p)
{
	int i = p.getPosX();
	int j = p.getPosY();

	int k = p.getKind();
	int o = p.getOrientation();

	switch(k)
	{
		case I:
			p.setColor(CYAN);
			break;
		case J:
			p.setColor(BLUE);
			break;
		case L:
			p.setColor(ORANGE);
			break;
		case O:
			p.setColor(YELLOW);
			break;
		case S:
			p.setColor(GREEN);
			break;
		case T:
			p.setColor(PURPLE);
			break;
		case Z:
			p.setColor(RED);
			break;
		default:
			break;
	}

	floodFill(i, j, PIVOT_X, PIVOT_Y, k, o, p.getColor());
}

void Board::clearPiece(Piece p)
{
	int i = p.getPosX();
	int j = p.getPosY();

	int k = p.getKind();
	int o = p.getOrientation();

	floodFill(i, j, PIVOT_X, PIVOT_Y, k, o, FREE);
}

void Board::newPiece(Piece p)
{
	p.setPosX(ORIGIN_X);
	p.setPosY(ORIGIN_Y);

	drawPiece(p);

	setCurrentPiece(p);
}

bool Board::isCurrentPieceMovable(int x, int y)
{
	clearPiece(currentPiece);

	bool movable = true;

	bool visited[SIZE][SIZE];

	for(int l = 0; l < SIZE; ++l)
		for(int m = 0; m < SIZE; ++m)
			visited[l][m] = false;

	int k = currentPiece.getKind();
	int o = currentPiece.getOrientation();

	flood(x, y, PIVOT_X, PIVOT_Y, k, o, movable, visited);

	drawPiece(currentPiece);

	return movable;
}

bool Board::isCurrentPieceRotable(int o)
{
	clearPiece(currentPiece);

	bool rotable = true;

	bool visited[SIZE][SIZE];

	for(int i = 0; i < SIZE; ++i)
		for(int j = 0; j < SIZE; ++j)
			visited[i][j] = false;

	int k = currentPiece.getKind();

	flood(currentPiece.getPosX(), currentPiece.getPosY(), PIVOT_X, PIVOT_Y, k, o, rotable, visited);

	drawPiece(currentPiece);

	return rotable;
}

void Board::moveCurrentPieceDown()
{
	int x = currentPiece.getPosX();
	int y = currentPiece.getPosY();

	if(isCurrentPieceMovable(x + 1, y))
	{
		clearPiece(currentPiece);
		currentPiece.setPosX(x + 1);

		drawPiece(currentPiece);
	}
}

void Board::moveCurrentPieceLeft()
{
	int x = currentPiece.getPosX();
	int y = currentPiece.getPosY();

	if(isCurrentPieceMovable(x, y - 1))
	{
		clearPiece(currentPiece);
		currentPiece.setPosY(y - 1);

		drawPiece(currentPiece);
	}
}

void Board::moveCurrentPieceRight()
{
	int x = currentPiece.getPosX();
	int y = currentPiece.getPosY();

	if(isCurrentPieceMovable(x, y + 1))
	{
		clearPiece(currentPiece);
		currentPiece.setPosY(y + 1);

		drawPiece(currentPiece);
	}
}

void Board::rotateCurrentPieceLeft()
{
	int o = currentPiece.getOrientation();

	if(o > 0)
		o--;
	else
		o = NB_ROTATIONS - 1;

	if(isCurrentPieceRotable(o))
	{
		clearPiece(currentPiece);

		currentPiece.setOrientation(o);
		drawPiece(currentPiece);
	}
}

void Board::rotateCurrentPieceRight()
{
	int o = currentPiece.getOrientation();

	if(o < NB_ROTATIONS - 1)
		o++;
	else
		o = 0;

	if(isCurrentPieceRotable(o))
	{
		clearPiece(currentPiece);
		currentPiece.setOrientation(o);

		drawPiece(currentPiece);
	}
}

void Board::deleteLine(int y)
{
	clearPiece(currentPiece);

	for(int j = y; j > 0; --j)
	{
		for(int i = 0; i < BOARD_WIDTH; ++i)
			area[i][j] = area[i][j-1];
	}

	drawPiece(currentPiece);
}

int Board::deletePossibleLines()
{
	int nbLinesDeleted = 0;

	for(int j = 0; j < BOARD_HEIGHT; ++j)
	{
		int i = 0;

		for(; i < BOARD_WIDTH && area[i][j] != FREE; ++i);

		if(i == BOARD_WIDTH)
		{
			nbLinesDeleted++;
			deleteLine(j);
		}
	}

	return nbLinesDeleted;
}

void Board::dropCurrentPiece()
{
	int x = currentPiece.getPosX();
	int y = currentPiece.getPosY();

	while(isCurrentPieceMovable(x++, y))
		moveCurrentPieceDown();
}

bool Board::isCurrentPieceFallen()
{
	int x = currentPiece.getPosX();
	int y = currentPiece.getPosY();

	if(isCurrentPieceMovable(x + 1, y))
		return false;

	return true;
}

bool Board::isGameOver()
{
	for(int i = 0; i < BOARD_WIDTH; ++i)
	{
		if(area[i][0] != FREE)
			return true;
	}

	return false;
}

void Board::clear()
{
	for(int i = 0; i < BOARD_WIDTH; ++i)
	{
		for(int j = 0; j < BOARD_HEIGHT; ++j)
			area[i][j] = FREE;
	}
}


