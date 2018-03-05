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
#include "ChessBoardFactory.hpp"



#include <limits>
#include <array>

enum class ChessGamePart
{
	OPENING,
	MID_GAME,
	END_GAME
};
class ChessBoardAnalysis;
class ChessBoardAnalysis
{
public:
	typedef std::shared_ptr<ChessBoardAnalysis> ptr;
	typedef signed long long weight_type;
	static const weight_type MIN_WEIGHT=std::numeric_limits<weight_type>::min();
	static const weight_type MAX_WEIGHT=std::numeric_limits<weight_type>::max();

	static unsigned long long constructed;
private:
	ChessBoard::ptr board;
	
	std::vector<ChessBoard::ptr>* possibleMoves;
	
	bool check;

	int8_t *underAttackByWhite; // [rank*w+file]
	int8_t *underAttackByBlack; // [rank*w+file]
	
	void calculatePossibleMoves_common();
	void calculatePossibleMoves_pawnfirst();
	void calculatePossibleMoves_enpassan();
	void calculatePossibleMoves_castling();
	
	static ChessBoardFactory factory;
public:
	ChessBoardAnalysis(ChessBoard::ptr board_);
	~ChessBoardAnalysis();
	void reset();

	bool isCheckMate() const;  // call to this function is underfined without calculatePossibleMoves()
	bool isCheck() const; // call to this function is underfined without calculatePossibleMoves()

	std::array<int16_t, KNOWN_CHESS_PIECE_COUNT> chessPiecesCount() const;
	
	weight_type chessPiecesWeight() const; // simple piece count (can be shown to user)
	weight_type chessPiecesWeight(const std::array<int16_t, KNOWN_CHESS_PIECE_COUNT> &count) const; // simple piece count (can be shown to user)
	weight_type chessPositionWeight(bool log=false) const; // analise the position, but not the tree
	
	weight_type chessPieceAttackedWeight() const;
	weight_type chessCentreControlWeight() const;
	
	ChessGamePart chessGamePart(const std::array<int16_t, KNOWN_CHESS_PIECE_COUNT> &count) const;
	weight_type chessKingPositionWeight(ChessGamePart gamePart) const;
	
	void calculatePossibleMoves();
	std::vector<ChessBoard::ptr> * const getPossibleMoves() const; // call to this function is underfined without calculatePossibleMoves()
	void clearPossibleMoves(ChessBoard::ptr toKeep = nullptr);

	
	ChessBoard::ptr getBoard() const;
};

constexpr ChessBoardAnalysis::weight_type
	PIECE_WEIGHT_MULTIPLIER=1000000,
	CHECKMATE_WEIGHT=-100000*PIECE_WEIGHT_MULTIPLIER, // mate is always more important
	BOARD_PAWN_WEIGHT=1*PIECE_WEIGHT_MULTIPLIER,
	BOARD_KNIGHT_WEIGHT=3*PIECE_WEIGHT_MULTIPLIER,
	BOARD_BISHOP_WEIGHT=3*PIECE_WEIGHT_MULTIPLIER,
	BOARD_ROOK_WEIGHT=5*PIECE_WEIGHT_MULTIPLIER,
	BOARD_QUEEN_WEIGHT=7*PIECE_WEIGHT_MULTIPLIER,
	BOARD_KING_WEIGHT=4*PIECE_WEIGHT_MULTIPLIER,
	
	PIECE_ATTACK_MULTIPLIER=-4,
	PIECE_DEFENCE_MUTIPLIER=1,
	PIECE_PRESENT_MILTIPLIER=10
	;

#endif
