/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSFUNCTIONS__
#define CHESSFUNCTIONS__

#include <vector>
#include <functional>
#include "ChessBoard.hpp"
#include "moveTemplate.hpp"

namespace ChessFunctions
{
	typedef std::function<void(char, int, char, int)> MoveRecordingFunction;
	void move(
		MoveRecordingFunction recFunTake,
		MoveRecordingFunction recFunDefend,
		const ChessBoard &cb, char file, int rank,
		const MoveTemplate& mt,
		bool canTake=true, bool canMoveToEmpty=true);

	bool ownPiece(ChessPiece cp, PlayerColour turn);
	int countPieces(const ChessBoard &cb, std::function<bool(ChessPiece)> test);
	int countPieces(const ChessBoard &cb, ChessPiece cp);
};

#endif