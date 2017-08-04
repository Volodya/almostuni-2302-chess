/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardFactory.hpp"
#include "ChessGameParameters.hpp"
#include <memory>
#include <cassert>
#include <iostream>

ChessBoard::ptr ChessBoardFactory::createBoard()
{
	ChessGameParameters::ptr param(new ChessGameParameters());
	param->setWidth(8);
	param->setHeight(8);
	param->addPossiblePieces(STANDARD_GAME_PIECES);
	
	ChessBoard::ptr cb(new ChessBoard(param));
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
	cm->previous=false;
	cb->move=cm;
	
	assert(cb->getTurn()==ChessPlayerColour::WHITE);
	
	return cb;
}

	// starting position: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
ChessBoard::ptr ChessBoardFactory::createBoard(std::string fen)
{
	ChessGameParameters::ptr param(new ChessGameParameters());
	param->setWidth(8);
	param->setHeight(8);
	param->addPossiblePieces(STANDARD_GAME_PIECES);

	ChessBoard::ptr cb(new ChessBoard(param));
	size_t file=0, rank=7;
	for(auto it=fen.begin(); it!=fen.end(); ++it)
	{
		if(*it==' ')
		{
			++it;
			if(*it=='w')
			{
				cb->turn=ChessPlayerColour::WHITE;
			}
			else
			{
				cb->turn=ChessPlayerColour::BLACK;
			}
			break;
		}
		else if(*it=='/')
		{
			file=0;
			--rank;
			continue;
		}
		if(*it>='1' && *it<='9')
		{
			file+=*it-'0';
		}
		else
		{
			cb->placePiecePos(file, rank, *it);
			++file;
		}
	}
	
	ChessMove::ptr cm(new ChessMove);
	
	cm->to=cb;
	cm->moveNum=0;
	cm->previous=false;
	cb->move=cm;
	
	return cb;
}

ChessBoard::ptr ChessBoardFactory::createBoard
  (ChessBoard::ptr fromBoard, char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	ChessBoard::ptr toBoard(new ChessBoard(fromBoard->param));
	
	// moving all pieces from the old board to new
	for(auto it=fromBoard->begin(); it!=fromBoard->end(); ++it)
	{
		toBoard->placePiece(it.getFile(), it.getRank(), *it);
	}
	// moving one of the pieces to the new position
	assert(fileFrom!=fileTo || rankFrom!=rankTo);
	auto piece = fromBoard->getPiece(fileFrom, rankFrom);
	auto takenPiece = toBoard->getPiece(fileTo, rankTo);
	toBoard->placePiece(fileTo, rankTo, piece);
	toBoard->placePiece(fileFrom, rankFrom, EMPTY_CELL);
	
	

	ChessMove::ptr curMove(new ChessMove);
	
	curMove->previous=true;
	curMove->from=fromBoard;
	curMove->to=toBoard;
	curMove->notation=std::string("")+
		piece+" "+
		fileFrom+std::to_string(rankFrom)+" "+
		fileTo+std::to_string(rankTo);
	curMove->moveNum=fromBoard->getMove()->moveNum+1;
	toBoard->move=curMove;
	toBoard->turn=!fromBoard->getTurn();
	
	assert(fromBoard->getTurn()!=toBoard->getTurn());
	assert(fromBoard->getTurn()==curMove->getTurn());
	
	return toBoard;
}