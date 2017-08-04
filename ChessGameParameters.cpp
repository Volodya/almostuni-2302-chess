/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessGameParameters.hpp"

#include "Log.hpp"

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
void ChessGameParameters::addPossiblePieces(std::vector<ChessPiece> cp)
{
	possiblePieces.resize(possiblePieces.size()+cp.size());
	std::copy(cp.begin(), cp.end(), possiblePieces.end());
}

uint8_t ChessGameParameters::getHeight() const
{
	return height;
}
uint8_t ChessGameParameters::getWidth() const
{
	return width;
}
size_t ChessGameParameters::getCellCount() const
{
	return size_t(height)*width;
}
const std::vector<ChessPiece> ChessGameParameters::getPossiblePieces() const
{
	return possiblePieces;
}
