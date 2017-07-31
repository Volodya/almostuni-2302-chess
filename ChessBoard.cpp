/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoard.hpp"
#include "ChessMove.hpp" // because it is not included in ChessBoard.hpp due to the circular dep

#include <iostream>
#include <cassert>

#include "moveTemplate.hpp"

// helper
std::map<ChessPiece, BitBoard> generateEmptyBitBoards(const std::vector<ChessPiece> possiblePieces, uint8_t height, uint8_t width)
{
	std::map<ChessPiece, BitBoard> result;
	for(auto it=possiblePieces.begin(); it!=possiblePieces.end(); ++it)
	{
		result.emplace(*it, height, width);
	}
	return result;
}

// class functions

ChessBoard::ChessBoard(uint8_t height, uint8_t width)
	: h(height), w(width), board(new ChessPiece[size_t(height)*width]), bitBoards(generateEmptyBitBoards({'p', 'P'}, height, width),
	  turn(ChessPlayerColour::WHITE), move(nullptr)
{
	for(uint8_t i=0; i<w; ++i)
	{
		for(uint8_t j=0; j<h; ++j)
		{
			board[size_t(i)*j] = ' ';
		}
	}
}
ChessBoard::ChessBoard(const ChessBoard& that)
	: h(that.h), w(that.w), board(new ChessPiece[size_t(that.h)*that.w]), bitBoards(that.bitBoards),
	  turn(that.turn), move(that.move)
{
	for(uint8_t i=0; i<w; ++i)
	{
		for(uint8_t j=0; j<h; ++j)
		{
			board[size_t(i)*j] = that.board[size_t(i)*j];
		}
	}
}

ChessBoard::~ChessBoard()
{
	delete[] board;
}

uint8_t ChessBoard::getHeight() const
{
	return h;
}
uint8_t ChessBoard::getWidth() const
{
	return w;
}


std::string ChessBoard::toFEN() const
{
	std::string result;
	
	int countEmpty=0;
	ChessPiece* c=board;
	for(int rank=7; rank>=0; --rank)
	{
		for(int file=0; file<8; ++file)
		{
			if(*c == EMPTY_CELL)
			{
				++countEmpty;
			}
			else
			{
				if(countEmpty>0)
				{
					result += std::to_string(countEmpty);
					countEmpty = 0;
				}
				result += *c;
			}
			
			++c;
		}
		if(countEmpty>0)
		{
			result += std::to_string(countEmpty);
			countEmpty = 0;
		}
		
		if(rank>0)
		{
			result += '/';
		}
	}
	
	result += ' ';
	
	result += turn==ChessPlayerColour::WHITE ? 'w' : 'b';
	
	result += ' ';
	
	return result;
}

void ChessBoard::debugPrint() const
{
	if(turn==ChessPlayerColour::WHITE)
	{
		std::cout << "White's turn" << std::endl;
	}
	else
	{
		std::cout << "Black's turn" << std::endl;
	}
	std::cout << ' ';
	for(uint8_t i=0; i<w; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	{
		ChessPiece* c=board;
		for(uint8_t rank=h; rank>0; --rank)
		{
			std::cout << rank;
			for(uint8_t file=0; file<w; ++file)
			{
				std::cout << ' ' << *c;
				++c;
			}
			std::cout << std::endl;
		}
	}
}

ChessPlayerColour ChessBoard::getTurn() const
{
	return turn;
}

void ChessBoard::placePiece(char file, int rank, ChessPiece piece)
{
	this->placePiecePos(file-'A', rank-1, piece);
}
void ChessBoard::placePiecePos(size_t file, size_t rank, ChessPiece piece)
{
	board[rank*w+file] = piece;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(size_t file, size_t rank) const
{
	return board[rank*w+file];
}

ChessMove::ptr ChessBoard::getMove() const
{
	return move;
}
bool ChessBoard::isEmpty(char file, int rank) const
{
	return this->isEmptyPos(file-'A', rank-1);
}
bool ChessBoard::isEmptyPos(size_t file, size_t rank) const
{
	return (board[rank*w+file] == ' ');
}


