/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDANALYSIS__
#define CHESSBOARDANALYSIS__

#include "config.hpp"

#include "ChessBoard.hpp"
#include "ChessMove.hpp"

class ChessBoardAnalysis;
class ChessBoardAnalysis
{
public:
	typedef std::shared_ptr<ChessBoardAnalysis> ptr;
	typedef signed long long weight_type;
	static unsigned long long constructed;
private:
	ChessBoard::ptr board;
	
	int8_t *underAttackByWhite; // [rank*w+file]
	int8_t *underAttackByBlack; // [rank*w+file]

	std::vector<ChessBoard::ptr>* possibleMoves;
	
	bool check;
	bool calculated;
public:
	ChessBoardAnalysis(ChessBoard::ptr board_);
	~ChessBoardAnalysis();

	bool isCheckMate() const;  // call to this function is underfined without calculatePossibleMoves()
	bool isCheck() const; // call to this function is underfined without calculatePossibleMoves()

	weight_type chessPiecesWeight() const; // simple piece count (can be shown to user)
	weight_type chessPositionWeight() const; // analise the position, but not the tree
	
	weight_type chessPieceAttackedWeight() const;
	weight_type chessCentreControlWeight() const;
	
	void calculatePossibleMoves();
	const std::vector<ChessBoard::ptr> * const getPossibleMoves() const; // call to this function is underfined without calculatePossibleMoves()
	
	ChessBoard::ptr getBoard() const;
};

constexpr ChessBoardAnalysis::weight_type
	CHECKMATE_WEIGHT=-100000000000, // mate is always more important
	BOARD_PAWN_WEIGHT=100000,
	BOARD_KNIGHT_WEIGHT=300000,
	BOARD_BISHOP_WEIGHT=300000,
	BOARD_ROOK_WEIGHT=500000,
	BOARD_QUEEN_WEIGHT=700000,
	BOARD_KING_WEIGHT=400000,
	
	PIECE_ATTACK_MULTIPLIER=-4,
	PIECE_DEFENCE_MUTIPLIER=1,
	PIECE_PRESENT_MILTIPLIER=10
	;

#endif