/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessGameParameters.hpp"

#include "Log.hpp"

void ChessGameParameters::setDimentions(uint8_t w, uint8_t h)
{
	this->width=w;
	this->height=h;
	this->cellCount = size_t(h)*size_t(w);
}
void ChessGameParameters::addPossiblePiece(ChessPiece cp)
{
	if(!possiblePieces)
	{
		possiblePieces = std::make_shared<std::vector<ChessPiece>>();
	}
	possiblePieces->push_back(cp);
}
void ChessGameParameters::addPossiblePieces(std::vector<ChessPiece> cp)
{
	if(!possiblePieces)
	{
		possiblePieces = std::make_shared<std::vector<ChessPiece>>();
	}
	possiblePieces->reserve(possiblePieces->size()+cp.size());
	std::copy(cp.begin(), cp.end(), std::back_inserter(*possiblePieces));
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
	return cellCount;
}
const std::shared_ptr<std::vector<ChessPiece>> ChessGameParameters::getPossiblePieces() const
{
	return possiblePieces;
}
