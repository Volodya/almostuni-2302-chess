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

class ChessBoard
{
	std::vector<std::shared_ptr<ChessBoard>> possibleMoves;
	bool possibleMovesCalculated;
	
	ChessPiece board[8][8]; // [rank][file]
	int8_t underAttackByWhite[8][8];
	int8_t underAttackByBlack[8][8];
	
	PlayerColour turn;
	bool check;
public:
	ChessBoard();
	bool isCheckMate() /*const*/;
	bool isCheck();
	bool isEmpty(char file, int rank) const;
	void placePiece(char file, int rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	std::shared_ptr<ChessBoard> move(char fileFrom, int rankFrom, char fileTo, int rankTo) const;
	PlayerColour getTurn() const;
	
	double weight() /*const*/; // analise the position
	
	void calculatePossibleMoves();
	std::vector<std::shared_ptr<ChessBoard>> getPossibleMoves() const;
	
	bool isPositionPossible() const;
	
	void debugPrint() const;
	
	ChessBoardIterator begin();
	ChessBoardIterator end();
	
	friend class ChessBoardIterator;
};

#endif