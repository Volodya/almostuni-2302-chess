/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSMOVE__
#define CHESSMOVE__

#include "config.hpp"

#include "ChessBoard.hpp"
#include "ChessPlayerColour.hpp"
#include "moveTemplate.hpp"
#include <string>
#include <functional>


class ChessMove
{
public:
	typedef std::function<void(ChessBoard::BoardPosition_t, ChessBoard::BoardPosition_t)> ChessMoveRecordingFunction;
	
	static bool isMovePossible(ChessBoard::ptr to);
	static void moveAttempts(
		const ChessMoveRecordingFunction &recFunTake,
		const ChessMoveRecordingFunction &recFunDefend,
		const ChessBoard &cb, ChessBoard::BoardPosition_t pos,
		const MoveTemplate& mt,
		bool canTake=true, bool canMoveToEmpty=true);
	
	static std::string getNotation(ChessBoard::ptr from, ChessBoard::ptr to);	
	static std::string generateCompleteMoveChain(ChessBoard::ptr finalBoard);
};

#endif