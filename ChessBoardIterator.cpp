/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardIterator.hpp"
#include "ChessBoard.hpp"

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