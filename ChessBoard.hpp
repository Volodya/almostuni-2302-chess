/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARD__
#define CHESSBOARD__

#include <iterator>
#include <vector>
#include <memory>
#include <cstdint>

class ChessMove;

typedef char ChessPiece;

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

class ChessBoard;

class ChessBoard
{
public:
	typedef std::shared_ptr<ChessBoard> ptr;

private:
	ChessPiece board[8][8]; // [rank][file]
	
	PlayerColour turn;
	
	ChessMove::ptr move;

	ChessBoard();
public:
	ChessBoard(const ChessBoard& that) = default;
	~ChessBoard();
	bool isEmpty(char file, int rank) const;
	void placePiece(char file, int rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	PlayerColour getTurn() const;
	ChessMove::ptr getMove() const;
	
	void debugPrint() const;
	
	ChessBoardIterator begin();
	ChessBoardConstIterator begin() const;
	ChessBoardIterator end();
	ChessBoardConstIterator end() const;
	
	friend class ChessBoardFactory;
	friend class ChessBoardIterator;
	friend class ChessBoardConstIterator;
	friend class ChessMove;
};

#endif