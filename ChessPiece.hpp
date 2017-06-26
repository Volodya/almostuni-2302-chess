/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSPIECE__
#define CHESSPIECE__

#include "ChessPlayerColour.hpp"

typedef char ChessPiece;

constexpr ChessPlayerColour getColour(const ChessPiece &cp)
{
	return
		(cp>='a' && cp<='z') ? ChessPlayerColour::BLACK : ChessPlayerColour::WHITE;
}

#endif