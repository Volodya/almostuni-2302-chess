/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDPIECE__
#define CHESSBOARDPIECE__

#include "ChessPiece.hpp"

struct ChessBoardPiece
{
	ChessPiece piece;
	size_t position;
	ChessBoardPiece() = default;
	ChessBoardPiece(ChessPiece piece_, size_t position);
	bool operator<(const ChessBoardPiece &that) const;
	size_t toArrayPos(size_t boardSize) const;
};	

#endif