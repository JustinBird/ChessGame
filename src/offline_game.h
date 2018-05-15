#pragma once
#include "board.h"

class offlinegame {
	board board_;
	Color turn_;

public:
	offlinegame();
	std::vector<ChessPiece*> get_board();
	void MovePiece(int, int);
	void NextTurn();

	Color get_turn();
	bool get_stalemate();
	bool get_checkmate();
};