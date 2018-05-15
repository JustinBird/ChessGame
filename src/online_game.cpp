#include "online_game.h"
#include <iostream>

//Constructs online game object with color.
onlinegame::onlinegame(Color c) {
	board_ = board();
	turn_ = WHITE;
	player_ = c;
}

Color onlinegame::get_color() {
	return player_;
}

std::vector<ChessPiece*> onlinegame::get_board() {
	return board_.get_board();
}

Color onlinegame::get_turn() {
	return turn_;
}

Color onlinegame::get_player() {
	return player_;
}

bool onlinegame::get_stalemate() {
	return board_.get_stalemate();
}

bool onlinegame::get_checkmate() {
	return board_.get_checkmate();
}

/**
 * Moves piece if it is the correct color for the turn.
 *
 * @param int Index of piece being moved.
 * @param int Index of square piece is being moved to. 
 */
void onlinegame::MovePiece(int old_index, int new_index) {
	if (board_.get_board().at(old_index)->get_color() == turn_) {
		if (board_.MovePiece(old_index, new_index)) {
			NextTurn();
		}
	}
	else {
		std::cout << "Not correct turn for that move" << std::endl;
	}
}

/**
 * Only allows moves that are the player's color. Called by mouse clicked.
 *
 * @param int Index of piece being moved.
 * @param int Index of square piece is being mvoed to.
 */
void onlinegame::PlayerMove(int old_index, int new_index) {
	if (board_.get_board().at(old_index)->get_color() == player_) {
		MovePiece(old_index, new_index);
	}
	else {
		std::cout << "Not correct turn for player move" << std::endl;
	}
}

/**
 * Only allows moves that are not the player's color. Called by network.
 *
 * @param int Index of piece being moved.
 * @param int Index of square piece is being mvoed to.
 */
void onlinegame::OpponentMove(int old_index, int new_index) {
	if (board_.get_board().at(old_index)->get_color() != player_) {
		MovePiece(old_index, new_index);
	}
	else {
		std::cout << "Not correct turn for opponent move" << std::endl;
	}
}

/**
 * Updates turn so opponent can move.
 */
void onlinegame::NextTurn() {
	if (turn_ == WHITE) {
		turn_ = BLACK;
	}
	else {
		turn_ = WHITE;
	}
}