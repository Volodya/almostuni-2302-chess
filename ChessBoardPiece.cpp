/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardPiece.hpp"

ChessBoardPiece::ChessBoardPiece(ChessPiece piece_, size_t position_)
	:piece(piece_), position(position_)
{}

bool ChessBoardPiece::operator <(const ChessBoardPiece &that) const
{
	return position < that.position || (position == that.position && piece < that.piece);
}