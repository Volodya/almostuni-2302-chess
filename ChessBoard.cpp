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

int ChessBoard::chessBoardCount = 0;

// class functions

ChessBoard::ChessBoard(ChessGameParameters::ptr param)
	: cellCount(param->getCellCount()),
	  width(param->getWidth()), height(param->getHeight()),
	  board(new ChessPiece[cellCount]),
	  enPassan(cellCount),
	  turn(ChessPlayerColour::WHITE), moveNum(0),
	  knownPossibleMoves(nullptr)
{
	++chessBoardCount;
	
	possiblePieces = param->getPossiblePieces();
	
	std::fill(board, board+cellCount, EMPTY_CELL);
	std::fill(whiteKingPos, whiteKingPos+3, cellCount);
	std::fill(blackKingPos, blackKingPos+3, cellCount);
	
	whiteCastling[0] = whiteCastling[1] = blackCastling[0] = blackCastling[1] = cellCount;
}
ChessBoard::ChessBoard(const ChessBoard::ptr& that)
	: cellCount(that->cellCount),
	  width(that->width), height(that->height),
	  board(new ChessPiece[cellCount]),
	  enPassan(cellCount),
	  turn(that->turn), moveNum(that->moveNum),
	  from(that),
	  knownPossibleMoves(nullptr)
{
	++chessBoardCount;
	
	std::copy(that->board, that->board+cellCount, this->board);

	std::copy(that->whiteKingPos, that->whiteKingPos+3, this->whiteKingPos);
	std::copy(that->blackKingPos, that->blackKingPos+3, this->blackKingPos);

	std::copy(that->whiteCastling, that->whiteCastling+2, this->whiteCastling);
	std::copy(that->blackCastling, that->blackCastling+2, this->blackCastling);
}

ChessBoard::~ChessBoard()
{
	--chessBoardCount;
	
	delete[] board;
	clearPossibleMoves();
}

ChessBoard::BoardPosition_t ChessBoard::getPos(const BoardPosition_t &file, const BoardPosition_t &rank) const
{
	return rank*width+file;
}

ChessBoard::BoardPosition_t ChessBoard::getCellCount() const
{
	return cellCount;
}
ChessBoard::BoardPosition_t ChessBoard::getHeight() const
{
	return height;
}
ChessBoard::BoardPosition_t ChessBoard::getWidth() const
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
	for(BoardPosition_t i=0; i<width; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	{
		ChessPiece c;
		for(BoardPosition_t rank=height; rank>0; --rank)
		{
			std::cout << (int)rank;
			for(BoardPosition_t file=0; file<width; ++file)
			{
				c = board[getPos(file, rank-1)];
				if(getPos(file, rank-1)==enPassan)
				{
					std::cout << ' ' << '*';
				}
				else
				{
					std::cout << ' ' << chessPieceStrings[c];
				}
			}
			std::cout << std::endl;
		}
	}
}

ChessBoard::ptr ChessBoard::getFrom() const
{
	return from;
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
void ChessBoard::placePiecePos(BoardPosition_t file, BoardPosition_t rank, ChessPiece piece)
{
	//Log::info("placePiecePos called without pos");
	this->placePiecePos(getPos(file, rank), piece);
}
void ChessBoard::placePiecePos(BoardPosition_t pos, ChessPiece piece)
{
	board[pos] = piece;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	//Log::info("getPiece called without pos");
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(ChessBoard::BoardPosition_t file, ChessBoard::BoardPosition_t rank) const
{
	//Log::info("getPiecePos called without pos");
	return getPiecePos(getPos(file, rank));
}
ChessPiece ChessBoard::getPiecePos(ChessBoard::BoardPosition_t pos) const
{
	return board[pos];
}

uint16_t ChessBoard::getMoveNum() const
{
	return moveNum;
}

bool ChessBoard::isEmpty(char file, int rank) const
{
	//Log::info("isEmpty called without pos");
	return this->isEmptyPos(file-'A', rank-1);
}
bool ChessBoard::isEmptyPos(ChessBoard::BoardPosition_t file, ChessBoard::BoardPosition_t rank) const
{
	//Log::info("isEmptyPos called without pos");
	return isEmptyPos(getPos(file, rank));
}
bool ChessBoard::isEmptyPos(ChessBoard::BoardPosition_t pos) const
{
	return board[pos] == EMPTY_CELL;
}

void ChessBoard::clearPossibleMoves(ChessBoard::ptr toKeep)
{
	if(!knownPossibleMoves) return;
	for(auto it=knownPossibleMoves->begin(), end=knownPossibleMoves->end(); it!=end; ++it)
	{
		if(*it != toKeep)
		{
			(*it)->from.reset();
			(*it)->clearPossibleMoves();
		}
	}
	if(toKeep)
	{
		knownPossibleMoves->resize(1);
		(*knownPossibleMoves)[0]=toKeep;
	}
	else
	{
		delete knownPossibleMoves;
		knownPossibleMoves=nullptr;
	}
}
