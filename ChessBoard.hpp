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

#include "BitBoard.hpp"
#include "ChessBoardHash.hpp"

class ChessMove;

class ChessBoard;

class ChessBoard
{
public:
	typedef std::shared_ptr<ChessBoard> ptr;
	typedef std::weak_ptr<ChessBoard> wptr;

	static std::shared_ptr<std::vector<ChessPiece>> possiblePieces;
private:
	size_t cellCount;
	size_t width, height;
	ChessPiece* board; // [rank*w+file]
	//std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> bitBoards;
	
	ChessPlayerColour turn;
	
	std::shared_ptr<ChessMove> move; // ChessMove::ptr
	
	std::unique_ptr<ChessBoardHash> hash;
	static std::vector<ChessBoardHash *> pieceHashes;
	void initialisePieceHashes();
	
	ChessBoard() = delete;
	ChessBoard(const ChessBoard& that) = delete;
	ChessBoard(ChessGameParameters::ptr param_);
	ChessBoard(const ChessBoard& that, std::shared_ptr<ChessMove> move_);
	
	size_t toPieceHashArrayPos(const ChessPiece &piece_, const size_t &position);
public:
	~ChessBoard();
	
	size_t getCellCount() const;
	size_t getHeight() const;
	size_t getWidth() const;

	bool isEmpty(char file, int rank) const;
	bool isEmptyPos(size_t file, size_t rank) const;
	void placePiece(char file, int rank, ChessPiece piece);
	void placePiecePos(size_t file, size_t rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	ChessPiece getPiecePos(size_t file, size_t rank) const;
	ChessPlayerColour getTurn() const;
	std::shared_ptr<ChessMove> getMove() const; // ChessMove::ptr
	ChessBoardHash getHash() const;
	
	size_t getPos(size_t file, size_t rank) const;

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