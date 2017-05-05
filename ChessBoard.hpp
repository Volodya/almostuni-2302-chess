#ifndef CHESSBOARD__
#define CHESSBOARD__

#include <iterator>

enum PlayerColour
{
	WHITE, BLACK
};

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
	ChessPiece board[8][8];
	PlayerColour turn;
public:
	ChessBoard();
	bool isCheckMate();
	bool isCheck();
	bool isEmpty(char file, int rank);
	void placePiece(char file, int rank, ChessPiece piece);
	ChessPiece getPiece(char file, int rank) const;
	ChessBoard move(char fileFrom, int rankFrom, char fileTo, int rankTo);
	PlayerColour getTurn();
	void debugPrint();
	
	ChessBoardIterator begin();
	ChessBoardIterator end();
	
	friend class ChessBoardIterator;
};

#endif