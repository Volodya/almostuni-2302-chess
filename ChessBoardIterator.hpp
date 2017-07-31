/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDITERATOR__
#define CHESSBOARDITERATOR__

#include "config.hpp"

#include "ChessPiece.hpp"
#include <iterator>


class ChessBoard;

class ChessBoardIterator : public std::iterator<std::input_iterator_tag, ChessPiece>
{
	size_t pos;
	ChessBoard *cb;
public:
	ChessBoardIterator(const ChessBoardIterator& that) = default;
	
	bool operator!=(const ChessBoardIterator& that);
	bool operator==(const ChessBoardIterator& that);
	ChessBoardIterator::value_type operator*() const;
	ChessBoardIterator& operator++();
	
	int getRank() const;
	size_t getRankPos() const;
	char getFile() const;
	size_t getFilePos() const;
private:
	ChessBoardIterator(ChessBoard* cb_, size_t pos_);
	
	friend ChessBoard;
};
class ChessBoardConstIterator : public std::iterator<std::input_iterator_tag, ChessPiece>
{
	size_t pos;
	const ChessBoard * cb;
public:
	ChessBoardConstIterator(const ChessBoardConstIterator& that) = default;
	
	bool operator!=(const ChessBoardConstIterator& that);
	bool operator==(const ChessBoardConstIterator& that);
	ChessBoardConstIterator::value_type operator*() const;
	ChessBoardConstIterator& operator++();
	
	int getRank() const;
	size_t getRankPos() const;
	char getFile() const;
	size_t getFilePos() const;
private:
	ChessBoardConstIterator(const ChessBoard* cb_, size_t pos_);
	
	friend ChessBoard;
};

#endif