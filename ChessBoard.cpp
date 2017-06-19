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

#include "chessfunctions.hpp"
#include "moveTemplate.hpp"

ChessBoard::ChessBoard()
	: turn(WHITE), from(nullptr)
{
	for(int i=0; i<8; ++i)
	{
		for(int j=0; j<8; ++j)
		{
			board[i][j] = ' ';
		}
	}
}
ChessBoard::~ChessBoard()
{
}

void ChessBoard::debugPrint() const
{
	if(turn==WHITE)
	{
		std::cout << "White's turn" << std::endl;
	}
	else
	{
		std::cout << "Black's turn" << std::endl;
	}
	std::cout << ' ';
	for(int i=0; i<8; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	for(int rank=8; rank>0; --rank)
	{
		std::cout << rank;
		for(int file=0; file<8; ++file)
		{
			std::cout << ' ' << board[rank-1][file];
		}
		std::cout << std::endl;
	}
}

PlayerColour ChessBoard::getTurn() const
{
	return turn;
}

void ChessBoard::placePiece(char file, int rank, ChessPiece piece)
{
	board[rank-1][file-'A'] = piece;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return board[rank-1][file-'A'];
}
ChessMove::ptr ChessBoard::getMove() const
{
	return move;
}
bool ChessBoard::isEmpty(char file, int rank) const
{
	return (board[rank-1][file-'A'] == ' ');
}

bool ChessBoard::isCheckMate() const
{
	int count=0;
	if(turn==BLACK)
	{
		// count white pieces
		count=ChessFunctions::countPieces(*this, [](ChessPiece onBoard) {
			return onBoard=='p' || onBoard=='n' || onBoard=='b' || onBoard=='r' || onBoard=='q' || onBoard=='k';
		});
	}
	else
	{
		// count black pieces
		count=ChessFunctions::countPieces(*this, [](ChessPiece onBoard) {
			return onBoard=='P' || onBoard=='N' || onBoard=='B' || onBoard=='R' || onBoard=='Q' || onBoard=='K';
		});
	}
	
	if(count==0)
	{
		return true;
	}
	
	if(isCheck())
	{
		return possibleMoves.empty();
	}
	
	return false;
}

// ITERATOR

ChessBoardIterator ChessBoard::begin()
{
	return ChessBoardIterator(this, 0, 0);
}

ChessBoardIterator ChessBoard::end()
{
	return ChessBoardIterator(this, 8, 0);
}

ChessBoardIterator::ChessBoardIterator(ChessBoard* cb_, int rank_, int file_)
	: rank(rank_), file(file_), cb(cb_)
{}

bool ChessBoardIterator::operator!=(const ChessBoardIterator& that)
{
	return !operator==(that);
}
bool ChessBoardIterator::operator==(const ChessBoardIterator& that)
{
	return this->rank==that.rank && this->file==that.file;
}
ChessBoardIterator::value_type ChessBoardIterator::operator*() const
{
	return cb->board[rank][file];
}
ChessBoardIterator& ChessBoardIterator::operator++()
{
	++file;
	if(file>=8)
	{
		++rank;
		file=0;
	}
	return *this;
}

int ChessBoardIterator::getRank() const
{
	return rank+1;
}
char ChessBoardIterator::getFile() const
{
	return file+'A';
}


// CONST ITERATOR

ChessBoardConstIterator ChessBoard::begin() const
{
	return ChessBoardConstIterator(this, 0, 0);
}

ChessBoardConstIterator ChessBoard::end() const
{
	return ChessBoardConstIterator(this, 8, 0);
}

ChessBoardConstIterator::ChessBoardConstIterator(const ChessBoard* cb_, int rank_, int file_)
	: rank(rank_), file(file_), cb(cb_)
{}

bool ChessBoardConstIterator::operator!=(const ChessBoardConstIterator& that)
{
	return !operator==(that);
}
bool ChessBoardConstIterator::operator==(const ChessBoardConstIterator& that)
{
	return this->rank==that.rank && this->file==that.file;
}
ChessBoardConstIterator::value_type ChessBoardConstIterator::operator*() const
{
	return cb->board[rank][file];
}
ChessBoardConstIterator& ChessBoardConstIterator::operator++()
{
	++file;
	if(file>=8)
	{
		++rank;
		file=0;
	}
	return *this;
}

int ChessBoardConstIterator::getRank() const
{
	return rank+1;
}
char ChessBoardConstIterator::getFile() const
{
	return file+'A';
}