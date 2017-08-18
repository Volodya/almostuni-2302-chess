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

constexpr double
	CHECKMATE_WEIGHT=-100000000, // mate is always more important
	BOARD_PAWN_WEIGHT=100,
	BOARD_KNIGHT_WEIGHT=300,
	BOARD_BISHOP_WEIGHT=300,
	BOARD_ROOK_WEIGHT=500,
	BOARD_QUEEN_WEIGHT=700,
	BOARD_KING_WEIGHT=400,
	
	PIECE_ATTACK_MULTIPLIER=0.20,
	PIECE_DEFENCE_MUTIPLIER=0.05
	;
class ChessBoardAnalysis;
class ChessBoardAnalysis
{
public:
	typedef std::shared_ptr<ChessBoardAnalysis> ptr;
	
private:
	ChessBoard::ptr board;
	
	int8_t underAttackByWhite[8][8]; // [rank][file]
	int8_t underAttackByBlack[8][8]; // [rank][file]

	std::vector<ChessBoard::ptr> possibleMoves;
	
	bool check;
public:
	ChessBoardAnalysis(ChessBoard::ptr board_);
	~ChessBoardAnalysis();

	bool isCheckMate() const;  // call to this function is underfined without calculatePossibleMoves()
	bool isCheck() const; // call to this function is underfined without calculatePossibleMoves()

	double chessPiecesWeight() const; // simple piece count (can be shown to user)
	double chessPositionWeight() const; // analise the position, but not the tree
	
	double chessPieceAttackedWeight() const;
	double chessCentreControlWeight() const;
	
	void calculatePossibleMoves();
	std::vector<ChessBoard::ptr> getPossibleMoves() const;
	
	ChessBoard::ptr getBoard() const;
	ChessBoardHash getBoardHash() const;
};

#endif