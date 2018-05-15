#pragma once
#include <string>

//Type used for chess piece color
typedef enum {
	WHITE = 0,
	BLACK
} Color;

//Type used for chess piece type.
typedef enum {
	BISHOP = 0,
	KING,
	KNIGHT,
	PAWN,
	QUEEN,
	ROOK
} Piece;

class ChessPiece {
	Color color_;
	Piece piece_;

public:
	ChessPiece(Color, Piece);
	Color get_color();
	Piece get_piece();
	void set_color(Color);
	void set_piece(Piece);
	std::string GetImagePath();
};
