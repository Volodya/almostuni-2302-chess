/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSGAMEPARAMETERS__
#define CHESSGAMEPARAMETERS__

#include <vector>
#include <cstdint>
#include <memory>

#include "ChessPiece.hpp"

struct ChessGameParameters
{
public:
	typedef uint16_t BoardPosition_t;
	
	BoardPosition_t height;
	BoardPosition_t width;
	BoardPosition_t cellCount;
	std::vector<ChessPiece> possiblePieces;

	void setDimentions(BoardPosition_t w, BoardPosition_t h);
};

#endif