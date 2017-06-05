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

enum PlayerColour
{
	WHITE=1, BLACK=-1
};

constexpr double
	CHECKMATE_WEIGHT=-100000000, // mate is always more important
	BOARD_PAWN_WEIGHT=100,
	BOARD_KNIGHT_WEIGHT=300,
	BOARD_BISHOP_WEIGHT=300,
	BOARD_ROOK_WEIGHT=500,
	BOARD_QUEEN_WEIGHT=700
	;

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
	std::vector<ptr> possibleMoves;
	bool possibleMovesCalculated;
	
	ChessPiece board[8][8]; // [rank][file]
	int8_t underAttackByWhite[8][8];
	int8_t underAttackByBlack[8][8];
	
	PlayerColour turn;
	bool check;
	
	ChessBoard::ptr from;
public:
	ChessBoard();
	bool isCheckMate() const;  // call to this function is underfined without calculatePossibleMoves()
	bool isCheck() const; // call to this function is underfined without calculatePossibleMoves()
	bool isEmpty(char file, int rank) const;
	void placePiece(char file, int rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	PlayerColour getTurn() const;
	ChessBoard::ptr getFrom() const;
	
	double weight() /*const*/; // analise the position
	
	std::vector<ChessBoard::ptr> getPossibleMoves() const;
	
	bool isPositionPossible() const;
	
	void debugPrint() const;
	
	ChessBoardIterator begin();
	ChessBoardConstIterator begin() const;
	ChessBoardIterator end();
	ChessBoardConstIterator end() const;
	
	friend class ChessBoardIterator;
	friend class ChessBoardConstIterator;
	friend void calculatePossibleMoves(std::shared_ptr<ChessBoard> obj);
	friend ChessBoard::ptr move(
		ChessBoard::ptr from, char fileFrom, int rankFrom, char fileTo, int rankTo);
};

#endif