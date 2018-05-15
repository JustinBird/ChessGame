#pragma once
#include "board.h"
#include <iostream>

/**
* Converts an x and y coordinate to an index in the board array.
*
* @param int x coordinate.
* @param int y coordinate.
* @return Integer corresponding to an index in board array.
*/
int ToIndex(int x_coord, int y_coord) {
	return x_coord + (y_coord * kBoardLen);
}

/**
* Converts an index of board array to an x coordinate.
*
* @param int Index of board.
* @return X coordinate corresponding to index.
*/
int ToXCoord(int index) {
	return index % kBoardLen;
}

/**
* Converts an index of board array to an y coordinate.
*
* @param int Index of board.
* @return Y coordinate corresponding to index.
*/
int ToYCoord(int index) {
	return ((index - (index % kBoardLen)) / kBoardLen);
}

/**
 * Default board constructor.
 */
board::board() {
	board_ = new std::vector<ChessPiece*>(kBoardSquares);

	//Create black pieces.
	int boardIndex = kBlackPiecesStart;
	
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::ROOK);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::KNIGHT);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::BISHOP);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::QUEEN);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::KING);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::BISHOP);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::KNIGHT);
	board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::ROOK);

	//Adds black pawns.
	for (int pawnIndex = 0; pawnIndex < kBoardLen; pawnIndex++) {
		board_->at(boardIndex++) = new ChessPiece(Color::BLACK, Piece::PAWN);
	}

	//Create white pieces.
	boardIndex = kWhitePiecesStart;

	//Adds white pawns.
	for (int pawnIndex = 0; pawnIndex < kBoardLen; pawnIndex++) {
		board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::PAWN);
	}

	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::ROOK);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::KNIGHT);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::BISHOP);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::QUEEN);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::KING);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::BISHOP);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::KNIGHT);
	board_->at(boardIndex++) = new ChessPiece(Color::WHITE, Piece::ROOK);

	stalemate_ = false;
	checkmate_ = false;
}

std::vector<ChessPiece*> board::get_board() {
	return *board_;
}

bool board::get_stalemate() {
	return stalemate_;
}

bool board::get_checkmate() {
	return checkmate_;
}

/**
 * Returns board index of the king depending on color.
 *
 * @param Color Color of the king you want to find.
 * @return Board index of the king with color.
 */
int board::get_king(Color c) {
	for (int piece_index = 0; piece_index < kBoardSquares; piece_index++) {
		ChessPiece* piece = board_->at(piece_index);
		if (piece) {
			if (piece->get_color() == c && piece->get_piece() == KING) {
				return piece_index;
			}
		}
	}

	return -1;
}

/**
 * Determines if the piece at new_index is of the same color.
 *
 * @return Depending on if the same color or not.
 */
bool board::EndsOnSameColor(Color c, int old_index, int new_index) {
	ChessPiece* end_pos = board_->at(new_index);
	if (end_pos && end_pos->get_color() == c) {
		return true;
	}

	return false;
}

/**
 * Moves a piece on board depending on if it is a valid move.
 *
 * @return true if the piece moved, false if it didn't.
 */
bool board::MovePiece(int old_index, int new_index) {
	Color moved_piece_color = board_->at(old_index)->get_color();

	bool valid = ValidMove(old_index, new_index);
	if (valid) {
		if (board_->at(new_index)) {
			ChessPiece* temp = board_->at(new_index);
			board_->at(new_index);
			delete temp;
		}

		board_->at(new_index) = board_->at(old_index);
		board_->at(old_index) = nullptr;
	}

	Color opponent_color;
	if (moved_piece_color == WHITE) {
		opponent_color = BLACK;
	}
	else {
		opponent_color = WHITE;
	}
	bool has_moves = HasValidMove(opponent_color);

	if (!has_moves) {
		if (InCheck(opponent_color, get_king(opponent_color))) {
			checkmate_ = true;
		}
		else {
			stalemate_ = true;
		}
	}

	return valid;
}

/**
 * Calls corresponding move function depending on piece type to determine if move is ok.
 *
 * @return Boolean depending on validity of move.
 */
bool board::ValidMove(int old_index, int new_index) {
	ChessPiece p = *(board_->at(old_index));

	bool possible_move;
	if (p.get_piece() == BISHOP) {
		possible_move = MoveBishop(p.get_color(), old_index, new_index);
	}
	else if (p.get_piece() == KING) {
		possible_move = MoveKing(p.get_color(), old_index, new_index);
	}
	else if (p.get_piece() == KNIGHT) {
		possible_move = MoveKnight(p.get_color(), old_index, new_index);
	}
	else if (p.get_piece() == PAWN) {
		possible_move = MovePawn(p.get_color(), old_index, new_index);
	}
	else if (p.get_piece() == QUEEN) {
		possible_move = MoveQueen(p.get_color(), old_index, new_index);
	}
	else {
		possible_move = MoveRook(p.get_color(), old_index, new_index);
	}

	//Moves the piece and sees if it puts the king in check. Returns pieces back to their
	//original place.
	bool in_check;
	if (possible_move) {
		ChessPiece* temp = nullptr;
		if (board_->at(new_index)) {
			temp = board_->at(new_index);
		}

		board_->at(new_index) = board_->at(old_index);
		board_->at(old_index) = nullptr;

		in_check = InCheck(p.get_color(), get_king(p.get_color()));

		board_->at(old_index) = board_->at(new_index);
		board_->at(new_index) = temp;
		temp = nullptr;
		delete temp;
	}

	return possible_move && !in_check;
}

/**
 * Determines if a pawn move is valid.
 *
 * @return Boolean depending on move validity.
 */
bool board::MovePawn(Color c, int old_index, int new_index) {
	int old_x = ToXCoord(old_index);
	int old_y = ToYCoord(old_index);
	int new_x = ToXCoord(new_index);
	int new_y = ToYCoord(new_index);

	ChessPiece* in_front_left = nullptr;
	ChessPiece* in_front = nullptr;
	ChessPiece* in_front_right = nullptr;

	if (c == WHITE) {

		//Gets piece one space in front and to the left, if it exists.
		if (old_x - 1 >= 0 && old_y - 1 >= 0) {
			in_front_left = board_->at(ToIndex(old_x - 1, old_y - 1));
		}

		//Gets piece in one space in front, if it exists.
		if (old_y - 1 >= 0) {
			in_front = board_->at(ToIndex(old_x, old_y - 1));
		}

		//Gets piece one space in front and to the right, if it exists.
		if (old_x + 1 < 8 && old_y - 1 >= 0) {
			in_front_right = board_->at(ToIndex(old_x + 1, old_y - 1));
		}

		//If the pawn is in it's starting space it can move forward two spaces.
		if (old_x == new_x && old_y == 6) {
			ChessPiece* two_in_front = board_->at(ToIndex(old_x, old_y - 2));
			if (!in_front && !two_in_front) {
				if (new_y == 5 || new_y == 4) {
					return true;
				}
			}
		}

		//If there is an enemy piece one space in front and to the left the pawn can move there.
		if (in_front_left) {
			if (in_front_left->get_color() == BLACK) {
				if (old_x - new_x == 1 && old_y - new_y == 1) {
					return true;
				}
			}
		}

		//If there is an enemy piece one space in front and to the right the pawn can move there.
		if (in_front_right) {
			if (in_front_right->get_color() == BLACK) {
				if (new_x - old_x == 1 && old_y - new_y == 1) {
					return true;
				}
			}
		}

		//If the pawn has not passed any of the previous cases and there is a piece in front of it
		//there will be no possible moves for it to make.
		if (in_front) {
			return false;
		} 

		//Pawn can move one space forward.
		return old_x == new_x && (old_y - new_y) == 1;
	}
	else {
		//Gets piece one space in front and to the left, if it exists.
		if (old_x - 1 >= 0 && old_y + 1 < 8) {
			in_front_left = board_->at(ToIndex(old_x - 1, old_y + 1));
		}

		//Gets piece in one space in front, if it exists.
		if (old_y + 1 < 8) {
			in_front = board_->at(ToIndex(old_x, old_y + 1));
		}

		//Gets piece one space in front and to the right, if it exists.
		if (old_x + 1 < 8 && old_y + 1 < 8) {
			in_front_right = board_->at(ToIndex(old_x + 1, old_y + 1));
		}

		//If the pawn is in it's starting space it can move forward two spaces.
		if (old_x == new_x && old_y == 1) {
			ChessPiece* two_in_front = board_->at(ToIndex(old_x, old_y + 2));
			if (!in_front && !two_in_front) {
				if (new_y == 2 || new_y == 3) {
					return true;
				}
			}
		}

		//If there is an enemy piece one space in front and to the left the pawn can move there.
		if (in_front_left) {
			if (in_front_left->get_color() == WHITE) {
				if (old_x - new_x == 1 && new_y - old_y == 1) {
					return true;
				}
			}
		}

		//If there is an enemy piece one space in front and to the right the pawn can move there.
		if (in_front_right) {
			if (in_front_right->get_color() == WHITE) {
				if (new_x - old_x == 1 && new_y - old_y == 1) {
					return true;
				}
			}
		}

		//If the pawn has not passed any of the previous cases and there is a piece in front of it
		//there will be no possible moves for it to make.
		if (in_front) {
			return false;
		}

		//Pawn can move one space forward.
		return old_x == new_x && (new_y - old_y) == 1;
	}
}

/**
 * Determines if a rook move is valid.
 *
 * @return Boolean depending on move validity.
 */
bool board::MoveRook(Color c, int old_index, int new_index) {
	int old_x = ToXCoord(old_index);
	int old_y = ToYCoord(old_index);
	int new_x = ToXCoord(new_index);
	int new_y = ToYCoord(new_index);

	//Rook cannot change x and y position in the same move (color independent).
	if (old_x != new_x && old_y != new_y) {
		return false;
	}

	//Cannot end on a piece with the same color.
	if (EndsOnSameColor(c, old_index, new_index)) {
		return false;
	}

	//Runs through the spaces between starting and stoping squares to ensure there are no pieces
	//inbetween.
	//Handles x movement.
	if (old_x != new_x) {
		//Handles x momvement right.
		if (old_x < new_x) {
			//Checks no pieces between start and ending squares.
			for (int path_index = old_x + 1; path_index < new_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(path_index, new_y));
				if (path) {
					return false;
				}
			}

			return true;
		}
		//Handles x movement left.
		else {
			//Checks no pieces between start and ending squares.
			for (int path_index = new_x + 1; path_index < old_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(path_index, new_y));
				if (path) {
					return false;
				}
			}

			return true;
		}
	}
	//Handles y movement
	else {
		//Handles y momvement up.
		if (old_y < new_y) {
			//Checks no pieces between start and ending squares.
			for (int path_index = old_y + 1; path_index < new_y; path_index++) {
				ChessPiece* path = board_->at(ToIndex(new_x, path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
		//Handles y movement down.
		else {
			//Checks no pieces between start and ending squares.
			for (int path_index = new_y + 1; path_index < old_y; path_index++) {
				ChessPiece* path = board_->at(ToIndex(new_x, path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
	}
}

/**
 * Determines if a bishop move is valid.
 *
 * @return Boolean depending on move validity.
 */
bool board::MoveBishop(Color c, int old_index, int new_index) {
	int old_x = ToXCoord(old_index);
	int old_y = ToYCoord(old_index);
	int new_x = ToXCoord(new_index);
	int new_y = ToYCoord(new_index);

	//Must move the same amount of spaces in x and y direction.
	if (std::abs(old_x - new_x) != std::abs(old_y - new_y)) {
		return false;
	}

	//Cannot end on a piece with the same color.
	if (EndsOnSameColor(c, old_index, new_index)) {
		return false;
	}

	//Checks if the bishop is moving over any pieces.
	if (old_x < new_x) {
		if (old_y < new_y) {
			for (int path_index = 1; old_x + path_index < new_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(old_x + path_index, old_y + path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
		else {
			for (int path_index = 1; old_x + path_index < new_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(old_x + path_index, old_y - path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
	}
	else {
		if (old_y < new_y) {
			for (int path_index = 1; new_x + path_index < old_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(old_x - path_index, old_y + path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
		else {
			for (int path_index = 1; new_x + path_index < old_x; path_index++) {
				ChessPiece* path = board_->at(ToIndex(old_x - path_index, old_y - path_index));
				if (path) {
					return false;
				}
			}

			return true;
		}
	}
}

/**
 * Determines if a queen move is valid.
 *
 * @return Boolean depending on move validity.
 */
bool board::MoveQueen(Color c, int old_index, int new_index) {
	//If the move would be valid for a rook or a bishop it would be valid for a queen.
	return MoveRook(c, old_index, new_index) || MoveBishop(c, old_index, new_index);
}

/**
* Determines if a knight move is valid.
*
* @return Boolean depending on move validity.
*/
bool board::MoveKnight(Color c, int old_index, int new_index) {
	int old_x = ToXCoord(old_index);
	int old_y = ToYCoord(old_index);
	int new_x = ToXCoord(new_index);
	int new_y = ToYCoord(new_index);

	//Cannot end on a piece with the same color.
	if (EndsOnSameColor(c, old_index, new_index)) {
		return false;
	}

	//The knight can either move the max (2) in the x direction and the min (1) in the y or 
	//vice versa.
	if (std::abs(old_x - new_x) == kKnightMaxMove && std::abs(old_y - new_y) == kKnightMinMove) {
		return true;
	}
	else if (std::abs(old_x - new_x) == kKnightMinMove && std::abs(old_y - new_y) == kKnightMaxMove) {
		return true;
	}
	
	return false;
}

/**
* Determines if a knight move is valid.
*
* @return Boolean depending on move validity.
*/
bool board::MoveKing(Color c, int old_index, int new_index) {
	int old_x = ToXCoord(old_index);
	int old_y = ToYCoord(old_index);
	int new_x = ToXCoord(new_index);
	int new_y = ToYCoord(new_index);

	//Cannot end on a piece with the same color.
	if (EndsOnSameColor(c, old_index, new_index)) {
		return false;
	}

	//Can't move into check.
	if (InCheck(c, new_index)) {
		return false;
	}

	//King can, at most, move one space in x and y direction.
	if (std::abs(old_x - new_x) > 1 || std::abs(old_y - new_y) > 1) {
		return false;
	}

	return true;
}

/**
 * Checks if piece at index with color c would be in check.
 *
 * @param Color Color of piece.
 * @param Index Index of piece.
 * @return Bool True if in check, false if not.
 */
bool board::InCheck(Color c, int index) {
	int x_pos = ToXCoord(index);
	int y_pos = ToYCoord(index);

	//Checks for rooks and queens putting king in check.
	//Check for rooks and queens to the left.
	for (int lpath = x_pos - 1; lpath >= 0; lpath--) {
		ChessPiece* piece = board_->at(ToIndex(lpath, y_pos));
		if (piece) {
			if (piece->get_color() != c) {
				if (piece->get_piece() == ROOK || piece->get_piece() == QUEEN) {
					std::cout << "Rook causing check" << std::endl;
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	//Checks for rooks and queens to the right.
	for (int rpath = x_pos; rpath < kBoardLen; rpath++) {
		ChessPiece* piece = board_->at(ToIndex(rpath, y_pos));
		if (piece) {
			if (piece->get_color() != c) {
				if (piece->get_piece() == ROOK || piece->get_piece() == QUEEN) {
					std::cout << "Rook causing check" << std::endl;
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	//Checks for rooks and queens above.
	for (int upath = y_pos - 1; upath >= 0; upath--) {
		ChessPiece* piece = board_->at(ToIndex(x_pos, upath));
		if (piece) {
			if (piece->get_color() != c) {
				if (piece->get_piece() == ROOK || piece->get_piece() == QUEEN) {
					std::cout << "Rook causing check" << std::endl;
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	//Checks for rooks and queens below.
	for (int dpath = y_pos + 1; dpath < kBoardLen; dpath++) {
		ChessPiece* piece = board_->at(ToIndex(x_pos, dpath));
		if (piece) {
			if (piece->get_color() != c) {
				if (piece->get_piece() == ROOK || piece->get_piece() == QUEEN) {
					std::cout << "Rook causing check" << std::endl;
					return true;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	//Checks for pawns infront. Forwards is dependent on piece color.
	if (c == WHITE) {
		if (y_pos > 0) {
			if (x_pos > 0) {
				ChessPiece* piece = board_->at(ToIndex(x_pos - 1, y_pos - 1));
				if (piece) {
					if (piece->get_piece() == PAWN && piece->get_color() == BLACK) {
						std::cout << "Pawn causing check" << std::endl;
						return true;
					}
				}
			}

			if (x_pos < kBoardLen - 1) {
				ChessPiece* piece = board_->at(ToIndex(x_pos + 1, y_pos - 1));
				if (piece) {
					if (piece->get_piece() == PAWN && piece->get_color() == BLACK) {		
						std::cout << "Pawn causing check" << std::endl;
						return true;
					}
				}
			}
		}
	}
	else {
		if (y_pos < kBoardLen - 1) {
			if (x_pos > 0) {
				ChessPiece* piece = board_->at(ToIndex(x_pos - 1, y_pos + 1));
				if (piece) {
					if (piece->get_piece() == PAWN && piece->get_color() == WHITE) {
						std::cout << "Pawn causing check" << std::endl;
						return true;
					}
				}
			}

			if (x_pos < kBoardLen - 1) {
				ChessPiece* piece = board_->at(ToIndex(x_pos + 1, y_pos + 1));
				if (piece) {
					if (piece->get_piece() == PAWN && piece->get_color() == WHITE) {
						std::cout << "Pawn causing check" << std::endl;
						return true;
					}
				}
			}
		}
	}

	//Check for bishops and queen diagonal moves.
	//Checks for bishops and queens down and to the right.
	for (int path = 1; path < kBoardLen; path++) {
		if ((x_pos + path) < kBoardLen && (y_pos + path) < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + path, y_pos + path));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == BISHOP || piece->get_piece() == QUEEN) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			break;
		}
	}

	//Checks for bishops and queens down and to the left.
	for (int path = 1; path < kBoardLen; path++) {
		if ((x_pos - path) >= 0 && (y_pos + path) < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - path, y_pos + path));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == BISHOP || piece->get_piece() == QUEEN) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			break;
		}
	}

	//Checks for bishops and queens up and to the right.
	for (int path = 1; path < kBoardLen; path++) {
		if ((x_pos + path) < kBoardLen && (y_pos - path) >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + path, y_pos - path));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == BISHOP || piece->get_piece() == QUEEN) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			break;
		}
	}

	//Checks for bishops and queens up and to the left.
	for (int path = 1; path < kBoardLen; path++) {
		if ((x_pos - path) >= 0 && (y_pos - path) >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - path, y_pos - path));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == BISHOP || piece->get_piece() == QUEEN) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
		}
		else {
			break;
		}
	}

	//Checks for knights two above and one to the left or right.
	if (y_pos - kKnightMaxMove >= 0) {
		if (x_pos - kKnightMinMove >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - kKnightMinMove, y_pos - kKnightMaxMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}

		if (x_pos + kKnightMinMove < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + kKnightMinMove, y_pos - kKnightMaxMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}
	}

	//Checks for knights one above and two to the left or right.
	if (y_pos - kKnightMinMove >= 0) {
		if (x_pos - kKnightMaxMove >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - kKnightMaxMove, y_pos - kKnightMinMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}

		if (x_pos + kKnightMaxMove < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + kKnightMaxMove, y_pos - kKnightMinMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}
	}

	//Checks for knights two below and one to the left or right.
	if (y_pos + kKnightMaxMove < kBoardLen) {
		if (x_pos - kKnightMinMove >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - kKnightMinMove, y_pos + kKnightMaxMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}

		if (x_pos + kKnightMinMove < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + kKnightMinMove, y_pos + kKnightMaxMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}
	}

	//Checks for knights one below and two to the left or right.
	if (y_pos + kKnightMinMove < kBoardLen) {
		if (x_pos - kKnightMaxMove >= 0) {
			ChessPiece* piece = board_->at(ToIndex(x_pos - kKnightMaxMove, y_pos + kKnightMinMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}

		if (x_pos + kKnightMaxMove < kBoardLen) {
			ChessPiece* piece = board_->at(ToIndex(x_pos + kKnightMaxMove, y_pos + kKnightMinMove));
			if (piece) {
				if (piece->get_color() != c) {
					if (piece->get_piece() == KNIGHT) {
						return true;
					}
				}
			}
		}
	}

	//If none of the other checks have returned then the king is not in check.
	return false;
}

/**
 * Checks if color c has any valid moves.
 *
 * @param Color Color to check for who has valid moves.
 * @return Bool True if has valid moves, false if not.
 */
bool board::HasValidMove(Color c) {
	for (int piece_index = 0; piece_index < kBoardSquares; piece_index++) {
		ChessPiece* piece = board_->at(piece_index);
		if (piece) {
			if (piece->get_color() == c) {
				for (int possible_move = 0; possible_move < kBoardSquares; possible_move++) {
					if (ValidMove(piece_index, possible_move)) {
						std::cout << piece_index << ", " << possible_move << std::endl;
						return true;
					}
				}
			}
		}
	}

	return false;
}