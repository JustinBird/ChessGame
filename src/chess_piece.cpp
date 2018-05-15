#include "chess_piece.h"
#include <string>

/**
 * Constructs chess piece given color and piece.
 */
ChessPiece::ChessPiece(Color color, Piece piece) : color_(color), piece_(piece) {

}

Color ChessPiece::get_color() {
	return color_;
}

Piece ChessPiece::get_piece() {
	return piece_;
}

void ChessPiece::set_color(Color color) {
	color_ = color;
}

void ChessPiece::set_piece(Piece piece) {
	piece_ = piece;
}

/**
 * Returns the image path depending on the piece type and color.
 *
 * @return Image path as a string.
 */
std::string ChessPiece::GetImagePath() {
	if (piece_ == BISHOP) {
		if (color_ == 0) {
			return "resources/images/wbishop.png";
		}
		else {
			return "resources/images/bbishop.png";
		}
	}
	else if (piece_ == KING) {
		if (color_ == 0) {
			return "resources/images/wking.png";
		}
		else {
			return "resources/images/bking.png";
		}
	}
	else if (piece_ == KNIGHT) {
		if (color_ == 0) {
			return "resources/images/wknight.png";
		}
		else {
			return "resources/images/bknight.png";
		}
	}
	else if (piece_ == PAWN) {
		if (color_ == 0) {
			return "resources/images/wpawn.png";
		}
		else {
			return "resources/images/bpawn.png";
		}
	}
	else if (piece_ == QUEEN) {
		if (color_ == 0) {
			return "resources/images/wqueen.png";
		}
		else {
			return "resources/images/bqueen.png";
		}
	}
	else if (piece_ == ROOK) {
		if (color_ == 0) {
			return "resources/images/wrook.png";
		}
		else {
			return "resources/images/brook.png";
		}
	}
	else {
		return "";
	}
}
