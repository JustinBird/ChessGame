#pragma once
#include "chess_piece.h"
#include <vector>

//Height and width of board.
const int kBoardLen = 8;

//Number of board squares.
const int kBoardSquares = 64;

//Knight must move at least one square in x or y direction.
const int kKnightMinMove = 1;

//Knight must move no more than two spaces in x or y direction.
const int kKnightMaxMove = 2;

//Black pieces start from index 0.
const int kBlackPiecesStart = 0;

//White pieces start from index 48.
const int kWhitePiecesStart = 48;

//Helper functions
int ToIndex(int, int);
int ToXCoord(int);
int ToYCoord(int);

class board {
	std::vector<ChessPiece*>* board_;
	bool stalemate_;
	bool checkmate_;

public:
	board();

	std::vector<ChessPiece*> get_board();
	bool get_stalemate();
	bool get_checkmate();

	int get_king(Color);

	bool MovePiece(int, int);
	bool ValidMove(int, int);

	bool MovePawn(Color, int, int);
	bool MoveRook(Color, int, int);
	bool MoveBishop(Color, int, int);
	bool MoveQueen(Color, int, int);
	bool MoveKnight(Color, int, int);
	bool MoveKing(Color, int, int);

	bool InCheck(Color, int);
	bool HasValidMove(Color);

	bool EndsOnSameColor(Color, int, int);
};