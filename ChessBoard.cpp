#include "ChessBoard.hpp"
#include <iostream>


ChessBoard::ChessBoard()
{
	for(int i=0; i<8; ++i)
	{
		for(int j=0; j<8; ++j)
		{
			board[i][j] = ' ';
		}
	}
}

void ChessBoard::debugPrint()
{
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

PlayerColour ChessBoard::getTurn()
{
	return turn;
}

void ChessBoard::placePiece(char file, int rank, ChessPiece piece)
{
	board[rank-1][file-'A'] = piece;
}
ChessBoard ChessBoard::move(char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	ChessBoard result(*this);
	
	result.placePiece(fileTo, rankTo, this->getPiece(fileFrom, rankFrom));
	this->placePiece(fileFrom, rankFrom, ' ');
	
	return result;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return board[rank-1][file-'A'];
}
bool ChessBoard::isEmpty(char file, int rank)
{
	return (board[rank-1][file-'A'] == ' ');
}

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