/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessMove.hpp"

ChessBoard::ptr move(
	ChessBoard::ptr from_, char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	to = std::make_shared<ChessBoard>(*from_);
	
	to->placePiece(fileTo, rankTo, from_->getPiece(fileFrom, rankFrom));
	to->placePiece(fileFrom, rankFrom, ' ');
	
	to->turn = (from->turn_==WHITE) ? BLACK : WHITE;
	to->from = *this;
}
