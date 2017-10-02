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

#include "Log.hpp"

// static data members

std::shared_ptr<std::vector<ChessPiece>> ChessBoard::possiblePieces = nullptr;

// class functions

ChessBoard::ChessBoard(ChessGameParameters::ptr param)
	: cellCount(param->getCellCount()),
	  width(param->getWidth()), height(param->getHeight()),
	  board(new ChessPiece[cellCount]),
	  turn(ChessPlayerColour::WHITE), move(nullptr),
	  knownPossibleMoves(nullptr)
{
	possiblePieces = param->getPossiblePieces();
	
	std::fill(board, board+cellCount, EMPTY_CELL);
}
ChessBoard::ChessBoard(const ChessBoard& that, ChessMove::ptr move_)
	: cellCount(that.cellCount),
	  width(that.width), height(that.height),
	  board(new ChessPiece[cellCount]),
	  turn(that.turn), move(move_),
	  knownPossibleMoves(nullptr)
{
	std::copy(that.board, that.board+cellCount, this->board);
}

ChessBoard::~ChessBoard()
{
	delete[] board;
	clearPossibleMoves();
}

size_t ChessBoard::getPos(const size_t &file, const size_t &rank) const
{
	return rank*width+file;
}

size_t ChessBoard::getCellCount() const
{
	return cellCount;
}
size_t ChessBoard::getHeight() const
{
	return height;
}
size_t ChessBoard::getWidth() const
{
	return width;
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
	for(size_t i=0; i<width; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	{
		ChessPiece c;
		for(size_t rank=height; rank>0; --rank)
		{
			std::cout << (int)rank;
			for(size_t file=0; file<width; ++file)
			{
				c = board[getPos(file, rank-1)];
				std::cout << ' ' << chessPieceStrings[c];
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
	//Log::info("placePiece called without pos");
	this->placePiecePos(file-'A', rank-1, piece);
}
void ChessBoard::placePiecePos(size_t file, size_t rank, ChessPiece piece)
{
	//Log::info("placePiecePos called without pos");
	this->placePiecePos(getPos(file, rank), piece);
}
void ChessBoard::placePiecePos(size_t pos, ChessPiece piece)
{
	board[pos] = piece;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	//Log::info("getPiece called without pos");
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(size_t file, size_t rank) const
{
	//Log::info("getPiecePos called without pos");
	return getPiecePos(getPos(file, rank));
}
ChessPiece ChessBoard::getPiecePos(size_t pos) const
{
	return board[pos];
}

ChessMove::ptr ChessBoard::getMove() const
{
	return move;
}

bool ChessBoard::isEmpty(char file, int rank) const
{
	//Log::info("isEmpty called without pos");
	return this->isEmptyPos(file-'A', rank-1);
}
bool ChessBoard::isEmptyPos(size_t file, size_t rank) const
{
	//Log::info("isEmptyPos called without pos");
	return isEmptyPos(getPos(file, rank));
}
bool ChessBoard::isEmptyPos(size_t pos) const
{
	return board[pos] == EMPTY_CELL;
}

void ChessBoard::clearPossibleMoves(ChessBoard::ptr toKeep)
{
	for(auto it=knownPossibleMoves->begin(), end=knownPossibleMoves->end(); it!=end; ++it)
	{
		if(*it != toKeep)
		{
			(*it)->clearPossibleMoves();
		}
	}
	knownPossibleMoves->resize(1);
	(*knownPossibleMoves)[0]=toKeep; 
}
void ChessBoard::clearPossibleMoves()
{
	if(!knownPossibleMoves) return;
	for(auto it=knownPossibleMoves->begin(), end=knownPossibleMoves->end(); it!=end; ++it)
	{
		(*it)->clearPossibleMoves();
	}
	delete knownPossibleMoves;
	knownPossibleMoves=nullptr;
}