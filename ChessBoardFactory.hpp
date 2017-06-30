/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDFACTORY__
#define CHESSBOARDFACTORY__

#include "ChessBoard.hpp"
#include "ChessMove.hpp"

class ChessBoardFactory
{
public:
	ChessBoard::ptr createBoard();
	ChessBoard::ptr createBoard(ChessBoard::ptr fromBoard, char fileFrom, int rankFrom, char fileTo, int rankTo);
};

#endif