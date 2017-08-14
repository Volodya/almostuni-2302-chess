/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef MOVETEMPLATE__
#define MOVETEMPLATE__

#include "config.hpp"

#include <vector>
#include <map>
#include "ChessPiece.hpp"

typedef std::vector<std::vector<std::pair<int, int>>> MoveTemplate;

MoveTemplate combineTwo(const MoveTemplate& a, const MoveTemplate& b);

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
	{ std::pair<int, int>( 1,  1), std::pair<int, int>( 2,  2), std::pair<int, int>( 3,  3),
	  std::pair<int, int>( 4,  4), std::pair<int, int>( 5,  5), std::pair<int, int>( 6,  6),
	  std::pair<int, int>( 7,  7), std::pair<int, int>( 8,  8), std::pair<int, int>( 9,  9),
	  std::pair<int, int>(10, 10), std::pair<int, int>(11, 11), std::pair<int, int>(12, 12),
	  std::pair<int, int>(13, 13), std::pair<int, int>(14, 14), std::pair<int, int>(15, 15),
	  std::pair<int, int>(16, 16), std::pair<int, int>(17, 17), std::pair<int, int>(18, 18)
	},
	{ std::pair<int, int>( 1,  -1), std::pair<int, int>( 2,  -2), std::pair<int, int>( 3,  -3),
	  std::pair<int, int>( 4,  -4), std::pair<int, int>( 5,  -5), std::pair<int, int>( 6,  -6),
	  std::pair<int, int>( 7,  -7), std::pair<int, int>( 8,  -8), std::pair<int, int>( 9,  -9),
	  std::pair<int, int>(10, -10), std::pair<int, int>(11, -11), std::pair<int, int>(12, -12),
	  std::pair<int, int>(13, -13), std::pair<int, int>(14, -14), std::pair<int, int>(15, -15),
	  std::pair<int, int>(16, -16), std::pair<int, int>(17, -17), std::pair<int, int>(18, -18)
	},
	{ std::pair<int, int>( -1,  1), std::pair<int, int>( -2,  2), std::pair<int, int>( -3,  3),
	  std::pair<int, int>( -4,  4), std::pair<int, int>( -5,  5), std::pair<int, int>( -6,  6),
	  std::pair<int, int>( -7,  7), std::pair<int, int>( -8,  8), std::pair<int, int>( -9,  9),
	  std::pair<int, int>(-10, 10), std::pair<int, int>(-11, 11), std::pair<int, int>(-12, 12),
	  std::pair<int, int>(-13, 13), std::pair<int, int>(-14, 14), std::pair<int, int>(-15, 15),
	  std::pair<int, int>(-16, 16), std::pair<int, int>(-17, 17), std::pair<int, int>(-18, 18)
	},
	{ std::pair<int, int>( -1,  -1), std::pair<int, int>( -2,  -2), std::pair<int, int>( -3,  -3),
	  std::pair<int, int>( -4,  -4), std::pair<int, int>( -5,  -5), std::pair<int, int>( -6,  -6),
	  std::pair<int, int>( -7,  -7), std::pair<int, int>( -8,  -8), std::pair<int, int>( -9,  -9),
	  std::pair<int, int>(-10, -10), std::pair<int, int>(-11, -11), std::pair<int, int>(-12, -12),
	  std::pair<int, int>(-13, -13), std::pair<int, int>(-14, -14), std::pair<int, int>(-15, -15),
	  std::pair<int, int>(-16, -16), std::pair<int, int>(-17, -17), std::pair<int, int>(-18, -18)
	}
};

const MoveTemplate rookMove =
{
	{ std::pair<int, int>(0,  1), std::pair<int, int>(0,  2), std::pair<int, int>(0,  3),
	  std::pair<int, int>(0,  4), std::pair<int, int>(0,  5), std::pair<int, int>(0,  6),
	  std::pair<int, int>(0,  7), std::pair<int, int>(0,  8), std::pair<int, int>(0,  9),
	  std::pair<int, int>(0, 10), std::pair<int, int>(0, 11), std::pair<int, int>(0, 12),
	  std::pair<int, int>(0, 13), std::pair<int, int>(0, 14), std::pair<int, int>(0, 15),
	  std::pair<int, int>(0, 16), std::pair<int, int>(0, 17), std::pair<int, int>(0, 18)
	},
	{ std::pair<int, int>(0,  -1), std::pair<int, int>(2,  -2), std::pair<int, int>(3,  -3),
	  std::pair<int, int>(0,  -4), std::pair<int, int>(5,  -5), std::pair<int, int>(6,  -6),
	  std::pair<int, int>(0,  -7), std::pair<int, int>(8,  -8), std::pair<int, int>(9,  -9),
	  std::pair<int, int>(0, -10), std::pair<int, int>(1, -11), std::pair<int, int>(2, -12),
	  std::pair<int, int>(0, -13), std::pair<int, int>(4, -14), std::pair<int, int>(5, -15),
	  std::pair<int, int>(0, -16), std::pair<int, int>(7, -17), std::pair<int, int>(8, -18)
	},
	{ std::pair<int, int>( -1, 0), std::pair<int, int>( -2, 0), std::pair<int, int>( -3, 0),
	  std::pair<int, int>( -4, 0), std::pair<int, int>( -5, 0), std::pair<int, int>( -6, 0),
	  std::pair<int, int>( -7, 0), std::pair<int, int>( -8, 0), std::pair<int, int>( -9, 0),
	  std::pair<int, int>(-10, 0), std::pair<int, int>(-11, 0), std::pair<int, int>(-12, 0),
	  std::pair<int, int>(-13, 0), std::pair<int, int>(-14, 0), std::pair<int, int>(-15, 0),
	  std::pair<int, int>(-16, 0), std::pair<int, int>(-17, 0), std::pair<int, int>(-18, 0)
	},
	{ std::pair<int, int>( -1, 0), std::pair<int, int>( -2, 0), std::pair<int, int>( -3, 0),
	  std::pair<int, int>( -4, 0), std::pair<int, int>( -5, 0), std::pair<int, int>( -6, 0),
	  std::pair<int, int>( -7, 0), std::pair<int, int>( -8, 0), std::pair<int, int>( -9, 0),
	  std::pair<int, int>(-10, 0), std::pair<int, int>(-11, 0), std::pair<int, int>(-12, 0),
	  std::pair<int, int>(-13, 0), std::pair<int, int>(-14, 0), std::pair<int, int>(-15, 0),
	  std::pair<int, int>(-16, 0), std::pair<int, int>(-17, 0), std::pair<int, int>(-18, 0)
	}
};

const MoveTemplate queenMove = combineTwo(rookMove, bishopMove);

const MoveTemplate princessMove = combineTwo(bishopMove, knightMove);

const MoveTemplate empressMove = combineTwo(rookMove, knightMove);

const MoveTemplate amazonMove = combineTwo(queenMove, knightMove);

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
	const MoveTemplate* takeMove;
	const MoveTemplate* noTakeMove;
	const MoveTemplate* anyMove;
	
	ChessPieceParameters(bool differentMoveTypes_, const MoveTemplate* takeMove_, const MoveTemplate* noTakeMove_,
		const MoveTemplate* anyMove_);
	ChessPieceParameters(const MoveTemplate* takeMove_, const MoveTemplate* noTakeMove_);
	ChessPieceParameters(const MoveTemplate* anyMove_);
};

const std::map<ChessPiece, const ChessPieceParameters> moveParameters =
	{
			{'P', ChessPieceParameters(&pawnWhiteMoveTake, &pawnWhiteMoveNoTake)},
			{'p', ChessPieceParameters(&pawnBlackMoveTake, &pawnBlackMoveNoTake)},
			{'N', ChessPieceParameters(&knightMove)},
			{'n', ChessPieceParameters(&knightMove)},
			{'B', ChessPieceParameters(&bishopMove)},
			{'b', ChessPieceParameters(&bishopMove)},
			{'R', ChessPieceParameters(&rookMove)},
			{'r', ChessPieceParameters(&rookMove)},
			{'Q', ChessPieceParameters(&queenMove)},
			{'q', ChessPieceParameters(&queenMove)},
			{'K', ChessPieceParameters(&kingMove)},
			{'k', ChessPieceParameters(&kingMove)}
	};

#endif