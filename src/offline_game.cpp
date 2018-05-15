#include "offline_game.h"

//Default constructor.
offlinegame::offlinegame() {
	board_ = board();
	turn_ = WHITE;
}

std::vector<ChessPiece*> offlinegame::get_board() {
	return board_.get_board();
}

Color offlinegame::get_turn() {
	return turn_;
}

bool offlinegame::get_stalemate() {
	return board_.get_stalemate();
}

bool offlinegame::get_checkmate() {
	return board_.get_checkmate();
}

/**
 * Moves piece if it is the correct color for the turn.
 *
 * @param int Index of piece being moved.
 * @param int Index of square piece is being moved to.
 */
void offlinegame::MovePiece(int old_index, int new_index) {
	if (board_.get_board().at(old_index)->get_color() == turn_) {
		if (board_.MovePiece(old_index, new_index)) {
			NextTurn();
		}
	}
}

/**
* Updates turn so opponent can move.
*/
void offlinegame::NextTurn() {
	if (turn_ == WHITE) {
		turn_ = BLACK;
	}
	else {
		turn_ = WHITE;
	}
}