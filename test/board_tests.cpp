#include "catch.hpp"
#include "../src/board.h"

TEST_CASE("Pawn Moving") {
	board b;
	
	SECTION("Pawn can move one forward") {
		REQUIRE(b.MovePiece(8, 16));
	}
	
	SECTION("Pawn can move two forward") {
		REQUIRE(b.MovePiece(8, 24));
	}
}

TEST_CASE("Rook Moving") {
	board b;
	b.MovePiece(8, 24);

	SECTION("Rook can move forward") {
		REQUIRE(b.MovePiece(0, 16));
	}

	SECTION("Rook can move side to side") {
		b.MovePiece(0, 16);
		REQUIRE(b.MovePiece(16, 19));
	}
}

TEST_CASE("Knight Moving") {
	board b;

	SECTION("Knight can move two forward and one left") {
		REQUIRE(b.MovePiece(1, 16));
	}

	SECTION("Knight can move two forward and one right") {
		REQUIRE(b.MovePiece(1, 18));
	}
}

TEST_CASE("Bishop Moving") {
	board b;

	SECTION("Bishop can move left diagonally") {
		b.MovePiece(9, 17);
		REQUIRE(b.MovePiece(2, 9));
	}

	SECTION("Bishop can move right diagonally") {
		b.MovePiece(11, 19);
		REQUIRE(b.MovePiece(2, 11));
	}
}

TEST_CASE("King Moving") {
	board b;

	SECTION("King can move forward") {
		b.MovePiece(12, 20);
		REQUIRE(b.MovePiece(4, 12));
	}
}

TEST_CASE("Queen Moving") {
	board b;
	b.MovePiece(11, 27);

	SECTION("Queen can move forward") {
		REQUIRE(b.MovePiece(3, 19));
	}

	SECTION("Queen can move left") {
		b.MovePiece(3, 19);
		REQUIRE(b.MovePiece(19, 18));
	}

	SECTION("Queen can move right") {
		b.MovePiece(3, 19);
		REQUIRE(b.MovePiece(19, 20));
	}

	SECTION("Queen can move diagonally left") {
		b.MovePiece(3, 19);
		REQUIRE(b.MovePiece(19, 26));
	}

	SECTION("Queen can move forward") {
		b.MovePiece(3, 19);
		REQUIRE(b.MovePiece(19, 28));
	}
}