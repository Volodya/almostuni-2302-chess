/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDANALYSIS__
#define CHESSBOARDANALYSIS__

#include "ChessBoard.hpp"

constexpr double
	CHECKMATE_WEIGHT=-100000000, // mate is always more important
	BOARD_PAWN_WEIGHT=100,
	BOARD_KNIGHT_WEIGHT=300,
	BOARD_BISHOP_WEIGHT=300,
	BOARD_ROOK_WEIGHT=500,
	BOARD_QUEEN_WEIGHT=700
	;

class ChessBoardAnalysis
{
	ChessBoard::ptr board;
	
	int8_t underAttackByWhite[8][8];
	int8_t underAttackByBlack[8][8];

	std::vector<ChessMove::ptr> possibleMoves;
	
	bool check;
public:
	ChessBoardAnalysis(ChessBoard::ptr board_);

	bool isCheckMate() const;  // call to this function is underfined without calculatePossibleMoves()
	bool isCheck() const; // call to this function is underfined without calculatePossibleMoves()

	double chessPiecesWeight() const; // simple piece count (can be shown to user)
	double chessPositionWeight() const; // analise the position, but not the tree
	
	void calculatePossibleMoves();
	std::vector<ChessMove::ptr> getPossibleMoves() const;
	
	ChessBoard::ptr getBoard() const;
};

#endif