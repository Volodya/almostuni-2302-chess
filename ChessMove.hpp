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

class ChessMove;

class ChessMove
{
public:
	typedef ptr std::shared_ptr<ChessMove>;

private:
	ChessBoard::ptr from;
	ChessBoard::ptr to;
	
	std::string notation;
	int moveNum;
	ChessMove(); // for initialisation in the factory

public:
	bool isMovePossible() const;
	bool hasPrevious() const;
	
	ChessBoard::ptr getFrom() const;
	ChessBoard::ptr getTo() const;
	PlayerColour getTurn() const;
	
	friend class ChessBoardFactory;
};

#endif