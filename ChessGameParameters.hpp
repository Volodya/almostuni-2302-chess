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

class ChessGameParameters;

class ChessGameParameters
{
public:
	typedef std::shared_ptr<ChessGameParameters> ptr;
private:
	uint8_t height;
	uint8_t width;
	size_t cellCount;
	std::shared_ptr<std::vector<ChessPiece>> possiblePieces;
public:
	void setDimentions(uint8_t w, uint8_t h);
	void addPossiblePiece(ChessPiece cp);
	void addPossiblePieces(std::vector<ChessPiece> cp);
	
	uint8_t getHeight() const;
	uint8_t getWidth() const;
	size_t getCellCount() const;
	const std::shared_ptr<std::vector<ChessPiece>> getPossiblePieces() const;
};

#endif