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

typedef char ChessPiece;
constexpr ChessPiece EMPTY_CELL = ' ';

const std::vector<ChessPiece> STANDARD_GAME_PIECES = 
	{ 'p', 'r', 'n', 'b', 'q', 'k',
	  'P', 'R', 'N', 'B', 'Q', 'K'
	};
const std::vector<ChessPiece> CAPABLANCA_GAME_PIECES = 
	{ 'p', 'r', 'n', 'b', 'q', 'k', // todo more pieces
	  'P', 'R', 'N', 'B', 'Q', 'K'
	};

constexpr ChessPlayerColour getColour(const ChessPiece &cp)
{
	return
		(cp>='a' && cp<='z') ? ChessPlayerColour::BLACK : ChessPlayerColour::WHITE;
}

#endif