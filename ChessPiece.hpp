/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSPIECE__
#define CHESSPIECE__

#include "config.hpp"

#include "ChessPlayerColour.hpp"

#include <vector>
#include <string>

// white - odd
// black - even
const size_t KNOWN_CHESS_PIECE_COUNT = 19;
/*
enum ChessPiece
{
	EMPTY_CELL = 0,
	PAWN_WHITE = 1,
	PAWN_BLACK = 2,
	ROOK_WHITE = 3,
	ROOK_BLACK = 4,
	KNIGHT_WHITE = 5,
	KNIGHT_BLACK = 6,
	BISHOP_WHITE = 7,
	BISHOP_BLACK = 8,
	KING_WHITE = 9,
	KING_BLACK = 10,
	QUEEN_WHITE = 11,
	QUEEN_BLACK = 12,
	PRINCESS_WHITE = 13,
	PRINCESS_BLACK = 14,
	EMPRESS_WHITE = 15,
	EMPRESS_BLACK = 16,
	AMAZON_WHITE = 17,
	AMAZON_BLACK = 18
};
*/
typedef unsigned char ChessPiece;
constexpr ChessPiece
	EMPTY_CELL = 0,
	PAWN_WHITE = 1,
	PAWN_BLACK = 2,
	ROOK_WHITE = 3,
	ROOK_BLACK = 4,
	KNIGHT_WHITE = 5,
	KNIGHT_BLACK = 6,
	BISHOP_WHITE = 7,
	BISHOP_BLACK = 8,
	KING_WHITE = 9,
	KING_BLACK = 10,
	QUEEN_WHITE = 11,
	QUEEN_BLACK = 12,
	PRINCESS_WHITE = 13,
	PRINCESS_BLACK = 14,
	EMPRESS_WHITE = 15,
	EMPRESS_BLACK = 16,
	AMAZON_WHITE = 17,
	AMAZON_BLACK = 18;

const std::string chessPieceStrings[] = 
{
	" ",
	"P", "p",
	"R", "r",
	"N", "n",
	"B", "b",
	"K", "k",
	"Q", "q",
	"C", "c",
	"E", "e",
	"A", "a"
};


const std::vector<ChessPiece> STANDARD_GAME_PIECES = 
	{ PAWN_WHITE, ROOK_WHITE, KNIGHT_WHITE, BISHOP_WHITE, QUEEN_WHITE, KING_WHITE,
	  PAWN_BLACK, ROOK_BLACK, KNIGHT_BLACK, BISHOP_BLACK, QUEEN_BLACK, KING_BLACK,
	};
const std::vector<ChessPiece> CAPABLANCA_GAME_PIECES = 
	{ PAWN_WHITE, ROOK_WHITE, KNIGHT_WHITE, BISHOP_WHITE, QUEEN_WHITE, KING_WHITE, // TODO: more pieces
	  PAWN_BLACK, ROOK_BLACK, KNIGHT_BLACK, BISHOP_BLACK, QUEEN_BLACK, KING_BLACK,
	};

constexpr ChessPlayerColour getColour(const ChessPiece &cp)
{
	return
		(cp & 1) ? ChessPlayerColour::WHITE : ChessPlayerColour::BLACK;
}
constexpr ChessPiece charToChessPiece(const char cp)
{
	return
		cp=='P' ? PAWN_WHITE :
		cp=='R' ? ROOK_WHITE :
		cp=='N' ? KNIGHT_WHITE :
		cp=='B' ? BISHOP_WHITE :
		cp=='Q' ? QUEEN_WHITE :
		cp=='K' ? KING_WHITE :
		cp=='p' ? PAWN_BLACK :
		cp=='r' ? ROOK_BLACK :
		cp=='n' ? KNIGHT_BLACK :
		cp=='b' ? BISHOP_BLACK :
		cp=='q' ? QUEEN_BLACK :
		cp=='k' ? KING_BLACK :
		EMPTY_CELL;
}

#endif