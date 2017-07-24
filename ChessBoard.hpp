/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARD__
#define CHESSBOARD__

#include "config.hpp"

#include <vector>
#include <memory>
#include <cstdint>

#include "ChessBoardIterator.hpp"
#include "ChessPiece.hpp"

class ChessMove;

class ChessBoard;

class ChessBoard
{
public:
	typedef std::shared_ptr<ChessBoard> ptr;
	typedef std::weak_ptr<ChessBoard> wptr;

	static unsigned long long count;
private:
	ChessPiece board[8][8]; // [rank][file]
	
	ChessPlayerColour turn;
	
	std::shared_ptr<ChessMove> move; // ChessMove::ptr

	ChessBoard();
public:
	ChessBoard(const ChessBoard& that) = default;
	~ChessBoard();
	bool isEmpty(char file, int rank) const;
	void placePiece(char file, int rank, ChessPiece piece);
	void placePiecePos(size_t file, size_t rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	ChessPiece getPiecePos(size_t file, size_t rank) const;
	ChessPlayerColour getTurn() const;
	std::shared_ptr<ChessMove> getMove() const; // ChessMove::ptr
	
	void debugPrint() const;
	
	std::string toFEN() const;
	
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