/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef MOVETEMPLATE__
#define MOVETEMPLATE__

#include <vector>
#include <map>

typedef std::vector<std::vector<std::pair<int, int>>> MoveTemplate;

const MoveTemplate pawnWhiteMoveNoTake =
{
	{ std::pair<int, int>(0,1) }
};
const MoveTemplate pawnWhiteMoveTake =
{
	{ std::pair<int, int>(1,1) },
	{ std::pair<int, int>(-1,1) }
};
const MoveTemplate pawnBlackMoveNoTake =
{
	{ std::pair<int, int>(0,-1) }
};
const MoveTemplate pawnBlackMoveTake =
{
	{ std::pair<int, int>(1,-1) },
	{ std::pair<int, int>(-1,-1) }
};

const MoveTemplate knightMove =
{
	{ std::pair<int, int>(1, 2) },
	{ std::pair<int, int>(-1, 2) },
	{ std::pair<int, int>(1, -2) },
	{ std::pair<int, int>(-1, -2) },
	{ std::pair<int, int>(2, 1) },
	{ std::pair<int, int>(-2, 1) },
	{ std::pair<int, int>(2, -1) },
	{ std::pair<int, int>(-2, -1) }
};

const MoveTemplate bishopMove =
{
	{ std::pair<int, int>(1, 1), std::pair<int, int>(2, 2), std::pair<int, int>(3, 3), std::pair<int, int>(4, 4),
		std::pair<int, int>(5, 5), std::pair<int, int>(6, 6), std::pair<int, int>(7, 7) },
	{ std::pair<int, int>(1, -1), std::pair<int, int>(2, -2), std::pair<int, int>(3, -3), std::pair<int, int>(4, -4),
		std::pair<int, int>(5, -5), std::pair<int, int>(6, -6), std::pair<int, int>(7, -7) },
	{ std::pair<int, int>(-1, 1), std::pair<int, int>(-2, 2), std::pair<int, int>(-3, 3), std::pair<int, int>(-4, 4),
		std::pair<int, int>(-5, 5), std::pair<int, int>(-6, 6), std::pair<int, int>(-7, 7) },
	{ std::pair<int, int>(-1, -1), std::pair<int, int>(-2, -2), std::pair<int, int>(-3, -3), std::pair<int, int>(-4, -4),
		std::pair<int, int>(-5, -5), std::pair<int, int>(-6, -6), std::pair<int, int>(-7, -7) }
};

const MoveTemplate rookMove =
{
	{ std::pair<int, int>(1, 0), std::pair<int, int>(2, 0), std::pair<int, int>(3, 0), std::pair<int, int>(4, 0),
		std::pair<int, int>(5, 0), std::pair<int, int>(6, 0), std::pair<int, int>(7, 0) },
	{ std::pair<int, int>(-1, 0), std::pair<int, int>(-2, 0), std::pair<int, int>(-3, 0), std::pair<int, int>(-4, 0),
		std::pair<int, int>(-5, 0), std::pair<int, int>(-6, 0), std::pair<int, int>(-7, 0) },
	{ std::pair<int, int>(0, 1), std::pair<int, int>(0, 2), std::pair<int, int>(0, 3), std::pair<int, int>(0, 4),
		std::pair<int, int>(0, 5), std::pair<int, int>(0, 6), std::pair<int, int>(0, 7) },
	{ std::pair<int, int>(0, -1), std::pair<int, int>(0, -2), std::pair<int, int>(0, -3), std::pair<int, int>(0, -4),
		std::pair<int, int>(0, -5), std::pair<int, int>(0, -6), std::pair<int, int>(0, -7) }
};

const MoveTemplate queenMove =
{
	// like a rook
	{ std::pair<int, int>(1, 0), std::pair<int, int>(2, 0), std::pair<int, int>(3, 0), std::pair<int, int>(4, 0),
		std::pair<int, int>(5, 0), std::pair<int, int>(6, 0), std::pair<int, int>(7, 0) },
	{ std::pair<int, int>(-1, 0), std::pair<int, int>(-2, 0), std::pair<int, int>(-3, 0), std::pair<int, int>(-4, 0),
		std::pair<int, int>(-5, 0), std::pair<int, int>(-6, 0), std::pair<int, int>(-7, 0) },
	{ std::pair<int, int>(0, 1), std::pair<int, int>(0, 2), std::pair<int, int>(0, 3), std::pair<int, int>(0, 4),
		std::pair<int, int>(0, 5), std::pair<int, int>(0, 6), std::pair<int, int>(0, 7) },
	{ std::pair<int, int>(0, -1), std::pair<int, int>(0, -2), std::pair<int, int>(0, -3), std::pair<int, int>(0, -4),
		std::pair<int, int>(0, -5), std::pair<int, int>(0, -6), std::pair<int, int>(0, -7) },
	// like a bishop
	{ std::pair<int, int>(1, 1), std::pair<int, int>(2, 2), std::pair<int, int>(3, 3), std::pair<int, int>(4, 4),
		std::pair<int, int>(5, 5), std::pair<int, int>(6, 6), std::pair<int, int>(7, 7) },
	{ std::pair<int, int>(1, -1), std::pair<int, int>(2, -2), std::pair<int, int>(3, -3), std::pair<int, int>(4, -4),
		std::pair<int, int>(5, -5), std::pair<int, int>(6, -6), std::pair<int, int>(7, -7) },
	{ std::pair<int, int>(-1, 1), std::pair<int, int>(-2, 2), std::pair<int, int>(-3, 3), std::pair<int, int>(-4, 4),
		std::pair<int, int>(-5, 5), std::pair<int, int>(-6, 6), std::pair<int, int>(-7, 7) },
	{ std::pair<int, int>(-1, -1), std::pair<int, int>(-2, -2), std::pair<int, int>(-3, -3), std::pair<int, int>(-4, -4),
		std::pair<int, int>(-5, -5), std::pair<int, int>(-6, -6), std::pair<int, int>(-7, -7) }
};

const MoveTemplate kingMove =
{
	{ std::pair<int, int>(1, 0) },
	{ std::pair<int, int>(1, 1) },
	{ std::pair<int, int>(0, 1) },
	{ std::pair<int, int>(-1, 1) },
	{ std::pair<int, int>(-1, 0) },
	{ std::pair<int, int>(-1, -1) },
	{ std::pair<int, int>(0, -1) },
	{ std::pair<int, int>(1, -1) }
};

struct ChessPieceParameters
{
	bool isDifferentMoveTypes;
	MoveTemplate* takeMove;
	MoveTemplate* noTakeMove;
	MoveTemplate* anyMove;
	
	ChessPieceParameters(bool differentMoveTypes_, MoveTemplate* takeMove_, MoveTemplate* noTakeMove_,
		MoveTemplate* anyMove_);
	ChessPieceParameters(MoveTemplate* takeMove_, MoveTemplate* noTakeMove_);
	ChessPieceParameters(MoveTemplate* anyMove_);
};

std::map<ChessPiece, const ChessPieceParameters> moveParameters;

moveParameters['P'] = ChessPieceParameters(&pawnWhiteMoveTake, &pawnWhiteMoveNoTake);
moveParameters['p'] = ChessPieceParameters(&pawnBlackMoveTake, &pawnBlackMoveNoTake);
moveParameters['N'] = ChessPieceParameters(&knightMove);
moveParameters['n'] = ChessPieceParameters(&knightMove);
moveParameters['B'] = ChessPieceParameters(&bishopMove);
moveParameters['b'] = ChessPieceParameters(&bishopMove);
moveParameters['R'] = ChessPieceParameters(&rookMove);
moveParameters['r'] = ChessPieceParameters(&rookMove);
moveParameters['Q'] = ChessPieceParameters(&queenMove);
moveParameters['q'] = ChessPieceParameters(&queenMove);
moveParameters['K'] = ChessPieceParameters(&kingMove);
moveParameters['k'] = ChessPieceParameters(&kingMove);

#endif