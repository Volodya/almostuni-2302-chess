/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSMOVE__
#define CHESSMOVE__

#include "ChessBoard.hpp"
#include <string>

class ChessMove
{
	ChessBoard::ptr from;
	ChessBoard::ptr to;
	
	std::string notation;
	int moveNum;
public:
	ChessMove(ChessBoard::ptr from_, char fileFrom, int rankFrom, char fileTo, int rankTo);
	ChessMove(); // for initialisation
	bool isMovePossible() const;
	bool hasPrevious()
	
};

#endif