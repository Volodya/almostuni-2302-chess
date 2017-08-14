/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "moveTemplate.hpp"

ChessPieceParameters::ChessPieceParameters(bool differentMoveTypes_, const MoveTemplate* takeMove_,
	const MoveTemplate* noTakeMove_, const MoveTemplate* anyMove_)
		: isDifferentMoveTypes(differentMoveTypes_), takeMove(takeMove_), noTakeMove(noTakeMove_), anyMove(anyMove_)
{}

ChessPieceParameters::ChessPieceParameters(const MoveTemplate* takeMove_, const MoveTemplate* noTakeMove_)
	: ChessPieceParameters(true, takeMove_, noTakeMove_, nullptr)
{}

ChessPieceParameters::ChessPieceParameters(const MoveTemplate* anyMove_)
	: ChessPieceParameters(false, anyMove_, anyMove_, anyMove_)
{}

MoveTemplate combineTwo(const MoveTemplate& a, const MoveTemplate& b)
{
	MoveTemplate result;
	result.reserve(a.size() + b.size());
	result.insert(result.end(), a.begin(), a.end());
	result.insert(result.end(), b.begin(), b.end());
	result.shrink_to_fit();
	return result;
}
