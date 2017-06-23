/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDITERATOR__
#define CHESSBOARDITERATOR__

class ChessBoard;

class ChessBoardIterator : public std::iterator<std::input_iterator_tag, ChessPiece>
{
	int rank, file;
	ChessBoard *cb;
public:
	ChessBoardIterator(const ChessBoardIterator& that);
	
	bool operator!=(const ChessBoardIterator& that);
	bool operator==(const ChessBoardIterator& that);
	ChessBoardIterator::value_type operator*() const;
	ChessBoardIterator& operator++();
	
	int getRank() const;
	char getFile() const;
private:
	ChessBoardIterator(ChessBoard* cb_, int rank_, int file_);
	
	friend ChessBoard;
};
class ChessBoardConstIterator : public std::iterator<std::input_iterator_tag, ChessPiece>
{
	int rank, file;
	const ChessBoard * cb;
public:
	ChessBoardConstIterator(const ChessBoardConstIterator& that);
	
	bool operator!=(const ChessBoardConstIterator& that);
	bool operator==(const ChessBoardConstIterator& that);
	ChessBoardConstIterator::value_type operator*() const;
	ChessBoardConstIterator& operator++();
	
	int getRank() const;
	char getFile() const;
private:
	ChessBoardConstIterator(const ChessBoard* cb_, int rank_, int file_);
	
	friend ChessBoard;
};

#endif