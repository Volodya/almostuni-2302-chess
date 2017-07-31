/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardIterator.hpp"
#include "ChessBoard.hpp"


ChessBoardIterator ChessBoard::begin()
{
	return ChessBoardIterator(this, 0);
}
ChessBoardConstIterator ChessBoard::begin() const
{
	return ChessBoardConstIterator(this, 0);
}

ChessBoardIterator ChessBoard::end()
{
	return ChessBoardIterator(this, size_t(this->getHeight())*this->getWidth());
}
ChessBoardConstIterator ChessBoard::end() const
{
	return ChessBoardConstIterator(this, size_t(this->getWidth())*this->getHeight());
}

ChessBoardIterator::ChessBoardIterator(ChessBoard* cb_, size_t pos_)
	: pos(pos_), cb(cb_)
{}
ChessBoardConstIterator::ChessBoardConstIterator(const ChessBoard* cb_, size_t pos_)
	: pos(pos_), cb(cb_)
{}

bool ChessBoardIterator::operator!=(const ChessBoardIterator& that)
{
	return !operator==(that);
}
bool ChessBoardConstIterator::operator!=(const ChessBoardConstIterator& that)
{
	return !operator==(that);
}

bool ChessBoardIterator::operator==(const ChessBoardIterator& that)
{
	return this->pos == that.pos;
}
bool ChessBoardConstIterator::operator==(const ChessBoardConstIterator& that)
{
	return this->pos == that.pos;
}

ChessBoardIterator::value_type ChessBoardIterator::operator*() const
{
	return cb->board[pos];
}
ChessBoardConstIterator::value_type ChessBoardConstIterator::operator*() const
{
	return cb->board[pos];
}

ChessBoardIterator& ChessBoardIterator::operator++()
{
	++pos;
	return *this;
}
ChessBoardConstIterator& ChessBoardConstIterator::operator++()
{
	++pos;
	return *this;
}

int ChessBoardIterator::getRank() const
{
	return getRankPos()+1;
}
int ChessBoardConstIterator::getRank() const
{
	return getRankPos()+1;
}

size_t ChessBoardIterator::getRankPos() const
{
	return pos / cb->getWidth();
}
size_t ChessBoardConstIterator::getRankPos() const
{
	return pos / cb->getWidth();
}

char ChessBoardIterator::getFile() const
{
	return getFilePos()+'A';
}
char ChessBoardConstIterator::getFile() const
{
	return getFilePos()+'A';
}

size_t ChessBoardIterator::getFilePos() const
{
	return pos % cb->getWidth();
}
size_t ChessBoardConstIterator::getFilePos() const
{
	return pos % cb->getWidth();
}