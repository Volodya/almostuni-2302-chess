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

class ChessMove;
class ChessBoardAnalysis;

class ChessBoard;

class ChessBoard
{
public:
	typedef std::shared_ptr<ChessBoard> ptr;
	typedef std::weak_ptr<ChessBoard> wptr;
	
	typedef uint16_t BoardPosition_t;

	static std::shared_ptr<std::vector<ChessPiece>> possiblePieces;
	
	static int chessBoardCount;
private:
	BoardPosition_t cellCount;
	BoardPosition_t width, height;
	ChessPiece* board; // [rank*w+file]
	BoardPosition_t enPassan;
	BoardPosition_t whiteKingPos[3]; // position, file, rank
	BoardPosition_t blackKingPos[3]; // position, file, rank
	BoardPosition_t whiteCastling[2];
	BoardPosition_t blackCastling[2];
	
	ChessPlayerColour turn;
	
	std::shared_ptr<ChessMove> move; // ChessMove::ptr
	
	std::vector<ChessBoard::ptr>* knownPossibleMoves;
	bool knownCheck;	
	
	ChessBoard() = delete;
	ChessBoard(const ChessBoard& that) = delete;
	ChessBoard(ChessGameParameters::ptr param_);
	ChessBoard(const ChessBoard& that, std::shared_ptr<ChessMove> move_);
public:
	~ChessBoard();
	
	BoardPosition_t getCellCount() const;
	BoardPosition_t getHeight() const;
	BoardPosition_t getWidth() const;

	bool isEmpty(char file, int rank) const;
	bool isEmptyPos(BoardPosition_t file, BoardPosition_t rank) const;
	bool isEmptyPos(BoardPosition_t pos) const;
	void placePiece(char file, int rank, ChessPiece piece);
	void placePiecePos(BoardPosition_t file, BoardPosition_t rank, ChessPiece piece);
	void placePiecePos(BoardPosition_t pos, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	ChessPiece getPiecePos(BoardPosition_t file, BoardPosition_t rank) const;
	ChessPiece getPiecePos(BoardPosition_t pos) const;
	ChessPlayerColour getTurn() const;
	std::shared_ptr<ChessMove> getMove() const; // ChessMove::ptr
	
	BoardPosition_t getPos(const BoardPosition_t &file, const BoardPosition_t &rank) const;

	void debugPrint() const;
	
	std::string toFEN() const;
	
	ChessBoardIterator begin();
	ChessBoardConstIterator begin() const;
	ChessBoardIterator end();
	ChessBoardConstIterator end() const;
	
	void clearPossibleMoves(ChessBoard::ptr toKeep = nullptr);
	
	friend class ChessBoardFactory;
	friend class ChessBoardIterator;
	friend class ChessBoardConstIterator;
	friend class ChessMove;
	friend class ChessBoardAnalysis;
};

#endif