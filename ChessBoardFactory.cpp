/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardFactory.hpp"
#include <memory>

ChessBoard::ptr ChessBoardFactory::createBoard()
{
	ChessBoard::ptr cb(new ChessBoard);
	// standard chess board
	cb->placePiece('A', 2, 'P');
	cb->placePiece('B', 2, 'P');
	cb->placePiece('C', 2, 'P');
	cb->placePiece('D', 2, 'P');
	cb->placePiece('E', 2, 'P');
	cb->placePiece('F', 2, 'P');
	cb->placePiece('G', 2, 'P');
	cb->placePiece('H', 2, 'P');
	cb->placePiece('A', 1, 'R');
	cb->placePiece('B', 1, 'N');
	cb->placePiece('C', 1, 'B');
	cb->placePiece('D', 1, 'Q');
	cb->placePiece('E', 1, 'K');
	cb->placePiece('F', 1, 'B');
	cb->placePiece('G', 1, 'N');
	cb->placePiece('H', 1, 'R');
	cb->placePiece('A', 7, 'p');
	cb->placePiece('B', 7, 'p');
	cb->placePiece('C', 7, 'p');
	cb->placePiece('D', 7, 'p');
	cb->placePiece('E', 7, 'p');
	cb->placePiece('F', 7, 'p');
	cb->placePiece('G', 7, 'p');
	cb->placePiece('H', 7, 'p');
	cb->placePiece('A', 8, 'r');
	cb->placePiece('B', 8, 'n');
	cb->placePiece('C', 8, 'b');
	cb->placePiece('D', 8, 'q');
	cb->placePiece('E', 8, 'k');
	cb->placePiece('F', 8, 'b');
	cb->placePiece('G', 8, 'n');
	cb->placePiece('H', 8, 'r');

	ChessMove::ptr cm(new ChessMove);
	
	cm->to=cb;
	cm->moveNum=0;
	cb->from=cm;
	
	return cb;
}

ChessBoard::ptr ChessBoardFactory::createBoard
  (ChessBoard::ptr from, char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	ChessBoard::ptr cb(new ChessBoard);

	ChessMove::ptr cm(new ChessMove);
	
	cm->from=from;
	cm->to=cb;
	cm->moveNum=from->getMove()->moveNum+1;
	cb->from=cm;
	
	return cb;
}