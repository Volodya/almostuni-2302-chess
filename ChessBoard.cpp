/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardAnalysis.hpp"
#include "ChessBoard.hpp"
#include "ChessMove.hpp" // because it is not included in ChessBoard.hpp due to the circular dep

#include <iostream>
#include <cassert>

#include "moveTemplate.hpp"

#include "Log.hpp"

// static data members

ChessGameParameters ChessBoard::param;

int ChessBoard::chessBoardCount = 0;
int ChessBoard::chessBoardArrayCreateCount = 0;
int ChessBoard::chessBoardArrayRecreateAttemptCount = 0;
int ChessBoard::chessBoardArrayDeleteCount = 0;

// class functions

ChessBoard::ChessBoard()
	: board(new ChessPiece[param.cellCount]),
	  enPassan(param.cellCount),
	  moveNum(0), turn(ChessPlayerColour::WHITE),
	  analysis(nullptr)
{
	++chessBoardCount;
	
	std::fill(board, board+param.cellCount, EMPTY_CELL);
	std::fill(changes, changes+4, ChessBoardChange(param.cellCount, EMPTY_CELL));
	std::fill(whiteKingPos, whiteKingPos+3, param.cellCount);
	std::fill(blackKingPos, blackKingPos+3, param.cellCount);
	
	whiteCastling[0] = whiteCastling[1] = blackCastling[0] = blackCastling[1] = param.cellCount;
}
ChessBoard::ChessBoard(const ChessBoard::ptr& that)
	: board(nullptr),
	  enPassan(param.cellCount),
	  moveNum(that->moveNum), turn(that->turn),
	  from(that),
	  analysis(nullptr)
{
	++chessBoardCount;
	
	std::fill(changes, changes+4, ChessBoardChange(param.cellCount, EMPTY_CELL));

	std::copy(that->whiteKingPos, that->whiteKingPos+3, this->whiteKingPos);
	std::copy(that->blackKingPos, that->blackKingPos+3, this->blackKingPos);

	std::copy(that->whiteCastling, that->whiteCastling+2, this->whiteCastling);
	std::copy(that->blackCastling, that->blackCastling+2, this->blackCastling);
}

ChessBoard::~ChessBoard()
{
	--chessBoardCount;
	
	if(board)
	{
		delete[] board;
	}
	if(analysis)
	{
		delete analysis;
	}
}

void ChessBoard::makeIFrame()
{
	if(!board)
	{
		++chessBoardArrayCreateCount;
		board = new ChessPiece[param.cellCount];
		std::copy(from->board, from->board+param.cellCount, board);
		for(size_t i=0; i<4; ++i)
		{
			if(changes[i].pos==param.cellCount)
			{
				break;
			}
			board[changes[i].pos] = changes[i].piece;
		}
	}
	else
	{
		++chessBoardArrayRecreateAttemptCount;
	}
}

void ChessBoard::makePFrame()
{
	if(board)
	{
		++chessBoardArrayDeleteCount;
		delete[] board;
		board=nullptr;
	}
}

ChessBoard::BoardPosition_t ChessBoard::getPos(const BoardPosition_t &file, const BoardPosition_t &rank) const
{
	return rank*param.width+file;
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
	for(BoardPosition_t i=0; i<param.width; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	{
		ChessPiece c;
		for(BoardPosition_t rank=param.height; rank>0; --rank)
		{
			std::cout << (int)rank;
			for(BoardPosition_t file=0; file<param.width; ++file)
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

void ChessBoard::placePiece(const char &file, int const &rank, ChessPiece piece)
{
	//Log::info("placePiece called without pos");
	this->placePiecePos(file-'A', rank-1, std::move(piece));
}
void ChessBoard::placePiecePos(const BoardPosition_t &file, const BoardPosition_t &rank, ChessPiece piece)
{
	//Log::info("placePiecePos called without pos");
	this->placePiecePos(getPos(file, rank), std::move(piece));
}
void ChessBoard::placePiecePos(const BoardPosition_t &pos, ChessPiece piece)
{
	if(board)
	{
		board[pos] = std::move(piece);
	}
	for(size_t i=0; i<4; ++i)
	{
		if(changes[i].pos==param.cellCount)
		{
			changes[i].pos = pos;
			changes[i].piece = piece;
			break;
		}
		assert(i!=3); // array has filled up (we have more than 3 changes)
	}
}
ChessPiece ChessBoard::getPiece(const char &file, const int &rank) const
{
	//Log::info("getPiece called without pos");
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(const ChessBoard::BoardPosition_t &file, const ChessBoard::BoardPosition_t &rank) const
{
	//Log::info("getPiecePos called without pos");
	return getPiecePos(getPos(file, rank));
}
ChessPiece ChessBoard::getPiecePos(const ChessBoard::BoardPosition_t &pos) const
{
	assert(board!=nullptr);
	return board[pos];
}

uint16_t ChessBoard::getMoveNum() const
{
	return moveNum;
}

bool ChessBoard::isEmpty(const char &file, const int &rank) const
{
	//Log::info("isEmpty called without pos");
	return this->isEmptyPos(file-'A', rank-1);
}
bool ChessBoard::isEmptyPos(const ChessBoard::BoardPosition_t &file, const ChessBoard::BoardPosition_t &rank) const
{
	//Log::info("isEmptyPos called without pos");
	return isEmptyPos(getPos(file, rank));
}
bool ChessBoard::isEmptyPos(const ChessBoard::BoardPosition_t &pos) const
{
	return board[pos] == EMPTY_CELL;
}

void ChessBoard::clearPossibleMoves(ChessBoard::ptr toKeep)
{
	if(!analysis) return;
	analysis->clearPossibleMoves(toKeep);
}

ChessBoardAnalysis* ChessBoard::getAnalysis(ChessBoard::ptr& self)
{
	if(self->analysis==nullptr)
	{
		self->analysis = new ChessBoardAnalysis(self);
	}
	return self->analysis;
}