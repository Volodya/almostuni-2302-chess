/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessGameParameters.hpp"

void ChessGameParameters::setHeight(uint8_t h)
{
	this->height=h;
}
void ChessGameParameters::setWidth(uint8_t w)
{
	this->width=w;
}
void ChessGameParameters::addPossiblePiece(ChessPiece cp)
{
	possiblePieces.push_back(cp);
}

uint8_t ChessGameParameters::getHeight() const
{
	return height;
}
uint8_t ChessGameParameters::getWidth() const
{
	return width;
}
const std::vector<ChessPiece> ChessGameParameters::getPossiblePieces() const
{
	return possiblePieces;
}
