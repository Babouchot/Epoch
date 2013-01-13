/* board.h */
#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

enum  { FREE, FILLED };

const int BOARD_HEIGHT = 20;
const int BOARD_WIDTH = 10;

const int ORIGIN_X = 0;
const int ORIGIN_Y = 5;

class Board
{
private:
	Piece currentPiece;
	Piece ghostPiece;

	void flood(int i, int j, int px, int py, int k, int o, int value, bool visited[][SIZE]);
    void flood(int i, int j, int px, int py, int k, int o, bool &flag, bool visited[][SIZE]);
	void floodFill(int i, int j, int px, int py, int k, int o, int value);

public:
	int area[BOARD_WIDTH][BOARD_HEIGHT];

	Board();

	void setCurrentPiece(Piece p);
	Piece getCurrentPiece();

	void drawPiece(Piece p);
	void clearPiece(Piece p);

	void newPiece(Piece p);

    bool isCurrentPieceMovable(int x, int y);
	bool isCurrentPieceRotable(int o);

	void moveCurrentPieceDown();
	void moveCurrentPieceLeft();
	void moveCurrentPieceRight();

	void rotateCurrentPieceLeft();
	void rotateCurrentPieceRight();

	void deleteLine(int y);
	int deletePossibleLines();

	void dropCurrentPiece();
	bool isCurrentPieceFallen();

	void clear();
};

#endif

