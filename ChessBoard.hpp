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
#include <map>
#include <memory>
#include <cstdint>

#include "ChessBoardIterator.hpp"
#include "ChessPiece.hpp"
#include "ChessGameParameters.hpp"

class ChessBoardAnalysis;

class ChessBoard;

class ChessBoard
{
public:
	typedef std::shared_ptr<ChessBoard> ptr;
	typedef std::weak_ptr<ChessBoard> wptr;
	
	typedef ChessGameParameters::BoardPosition_t BoardPosition_t;

	static ChessGameParameters param;
		
	static int chessBoardCount;
private:
	ChessPiece* board; // [rank*w+file]

	BoardPosition_t enPassan;
	BoardPosition_t whiteKingPos[3]; // position, file, rank
	BoardPosition_t blackKingPos[3]; // position, file, rank
	BoardPosition_t whiteCastling[2];
	BoardPosition_t blackCastling[2];
	
	uint16_t moveNum;
	ChessPlayerColour turn;
	
	ChessBoard::ptr from;
	
	ChessBoardAnalysis* analysis;

	ChessBoard();
	ChessBoard(const ChessBoard& that) = delete;
	ChessBoard(const ptr& that);
public:
	~ChessBoard();
	
	bool isEmpty(char file, int rank) const;
	bool isEmptyPos(BoardPosition_t file, BoardPosition_t rank) const;
	bool isEmptyPos(BoardPosition_t pos) const;
	void placePiece(char file, int rank, ChessPiece piece);
	void placePiecePos(BoardPosition_t file, BoardPosition_t rank, ChessPiece piece);
	void placePiecePos(BoardPosition_t pos, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	ChessPiece getPiecePos(BoardPosition_t file, BoardPosition_t rank) const;
	ChessPiece getPiecePos(BoardPosition_t pos) const;
	ptr getFrom() const;
	ChessPlayerColour getTurn() const;
	uint16_t getMoveNum() const;
	
	BoardPosition_t getPos(const BoardPosition_t &file, const BoardPosition_t &rank) const;

	void debugPrint() const;
	
	std::string toFEN() const;
	
	ChessBoardIterator begin();
	ChessBoardConstIterator begin() const;
	ChessBoardIterator end();
	ChessBoardConstIterator end() const;
	
	static ChessBoardAnalysis* getAnalysis(ChessBoard::ptr& self);
	
	void clearPossibleMoves(ChessBoard::ptr toKeep = nullptr);
	
	friend class ChessBoardFactory;
	friend class ChessBoardIterator;
	friend class ChessBoardConstIterator;
	friend class ChessMove;
	friend class ChessBoardAnalysis;
};

#endif