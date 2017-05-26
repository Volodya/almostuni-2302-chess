/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSENGINE__
#define CHESSENGINE__

#include "ChessBoard.hpp"

class ChessEngine
{
	std::shared_ptr<ChessBoard> curPos;
public:
	void setCurPos(std::shared_ptr<ChessBoard> newPos);
	std::shared_ptr<ChessBoard> getCurPos() const;
	
	// TODO: implement calculation of the next move
};

#endif