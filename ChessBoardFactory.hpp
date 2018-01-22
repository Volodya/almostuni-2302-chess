/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDFACTORY__
#define CHESSBOARDFACTORY__

#include "config.hpp"

#include "ChessBoard.hpp"
#include "ChessMove.hpp"
#include <string>
#include <vector>

class ChessBoardFactory
{
	ChessBoard::ptr createBoard(ChessBoard::ptr fromBoard);
public:
	//static std::vector<std::weak_ptr<ChessBoard>> allBoards;
	ChessBoard::ptr createBoard();
	ChessBoard::ptr createBoard(std::string fen);
	ChessBoard::ptr createBoard(
		ChessBoard::ptr fromBoard,
		const size_t &posFrom, const size_t &posTo);
	ChessBoard::ptr createBoard(
		ChessBoard::ptr fromBoard,
		const size_t &posFrom1, const size_t &posTo1,
		const size_t &posFrom2, const size_t &posTo2);
};

#endif