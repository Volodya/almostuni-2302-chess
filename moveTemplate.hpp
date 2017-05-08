#ifndef MOVETEMPLATE__
#define MOVETEMPLATE__

/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include <vector>

typedef std::vector<std::vector<std::pair<int, int>>> MoveTemplate;

const MoveTemplate pawnWhiteMoveNotTake =
{
	{ std::pair<int, int>(0,1) }
};
const MoveTemplate pawnWhiteMoveTake =
{
	{ std::pair<int, int>(1,1), std::pair<int, int>(-1,1) }
};
const MoveTemplate pawnBlackMoveNotTake =
{
	{ std::pair<int, int>(0,-1) }
};
const MoveTemplate pawnBlackMoveTake =
{
	{ std::pair<int, int>(1,-1), std::pair<int, int>(-1,-1) }
};



#endif