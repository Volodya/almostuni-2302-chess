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
	return ChessBoardIterator(this, param.width, 0);
}
ChessBoardConstIterator ChessBoard::begin() const
{
	return ChessBoardConstIterator(this, param.width, 0);
}

ChessBoardIterator ChessBoard::end()
{
	return ChessBoardIterator(this, param.width, param.cellCount);
}
ChessBoardConstIterator ChessBoard::end() const
{
	return ChessBoardConstIterator(this, param.width, param.cellCount);
}

ChessBoardIterator::ChessBoardIterator(ChessBoard* cb_, size_t boardWidth_, size_t pos_)
	: pos(pos_), cb(cb_), curPiece(cb_->board + pos_), boardWidth(boardWidth_)
{}
ChessBoardConstIterator::ChessBoardConstIterator(const ChessBoard* cb_, size_t boardWidth_, size_t pos_)
	: pos(pos_), cb(cb_), curPiece(cb_->board + pos_), boardWidth(boardWidth_)
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
	return *curPiece;
}
ChessBoardConstIterator::value_type ChessBoardConstIterator::operator*() const
{
	return *curPiece;
}

ChessBoardIterator& ChessBoardIterator::operator++()
{
	++pos;
	++curPiece;
	return *this;
}
ChessBoardConstIterator& ChessBoardConstIterator::operator++()
{
	++pos;
	++curPiece;
	return *this;
}

size_t ChessBoardIterator::getPos() const
{
	return pos;
}
size_t ChessBoardConstIterator::getPos() const
{
	return pos;
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
	return pos / boardWidth;
}
size_t ChessBoardConstIterator::getRankPos() const
{
	return pos / boardWidth;
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
	return pos % boardWidth;
}
size_t ChessBoardConstIterator::getFilePos() const
{
	return pos % boardWidth;
}