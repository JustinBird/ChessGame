#pragma once
#include "board.h"

class onlinegame {
	board board_;
	Color turn_;
	Color player_;

public:
	Color get_color();
	onlinegame(Color);
	std::vector<ChessPiece*> get_board();
	void MovePiece(int, int);
	void PlayerMove(int, int);
	void OpponentMove(int, int);
	void NextTurn();

	Color get_turn();
	Color get_player();
	bool get_stalemate();
	bool get_checkmate();
};
