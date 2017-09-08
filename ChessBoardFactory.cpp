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

#include "Log.hpp"

ChessBoard::ptr ChessBoardFactory::createBoard()
{
	ChessGameParameters::ptr param(new ChessGameParameters());
	param->setDimentions(8, 8);
	param->addPossiblePieces(STANDARD_GAME_PIECES);
	
	ChessBoard::ptr cb(new ChessBoard(param));
	
	// standard chess board
	cb->placePiece('A', 2, PAWN_WHITE);
	cb->placePiece('B', 2, PAWN_WHITE);
	cb->placePiece('C', 2, PAWN_WHITE);
	cb->placePiece('D', 2, PAWN_WHITE);
	cb->placePiece('E', 2, PAWN_WHITE);
	cb->placePiece('F', 2, PAWN_WHITE);
	cb->placePiece('G', 2, PAWN_WHITE);
	cb->placePiece('H', 2, PAWN_WHITE);
	cb->placePiece('A', 1, ROOK_WHITE);
	cb->placePiece('B', 1, KNIGHT_WHITE);
	cb->placePiece('C', 1, BISHOP_WHITE);
	cb->placePiece('D', 1, QUEEN_WHITE);
	cb->placePiece('E', 1, KING_WHITE);
	cb->placePiece('F', 1, BISHOP_WHITE);
	cb->placePiece('G', 1, KNIGHT_WHITE);
	cb->placePiece('H', 1, ROOK_WHITE);
	cb->placePiece('A', 7, PAWN_BLACK);
	cb->placePiece('B', 7, PAWN_BLACK);
	cb->placePiece('C', 7, PAWN_BLACK);
	cb->placePiece('D', 7, PAWN_BLACK);
	cb->placePiece('E', 7, PAWN_BLACK);
	cb->placePiece('F', 7, PAWN_BLACK);
	cb->placePiece('G', 7, PAWN_BLACK);
	cb->placePiece('H', 7, PAWN_BLACK);
	cb->placePiece('A', 8, ROOK_BLACK);
	cb->placePiece('B', 8, KNIGHT_BLACK);
	cb->placePiece('C', 8, BISHOP_BLACK);
	cb->placePiece('D', 8, QUEEN_BLACK);
	cb->placePiece('E', 8, KING_BLACK);
	cb->placePiece('F', 8, BISHOP_BLACK);
	cb->placePiece('G', 8, KNIGHT_BLACK);
	cb->placePiece('H', 8, ROOK_BLACK);

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
	param->setDimentions(8, 8);
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
			cb->placePiecePos(file, rank, charToChessPiece(*it));
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
	ChessBoard::ptr toBoard(new ChessBoard(*fromBoard));
	
	// moving all pieces from the old board to new
	/*
	for(auto it=fromBoard->begin(); it!=fromBoard->end(); ++it)
	{
		toBoard->placePiece(it.getFile(), it.getRank(), *it);
	}
	*/
	// moving one of the pieces to the new position
	assert(fileFrom!=fileTo || rankFrom!=rankTo);
	auto piece = fromBoard->getPiece(fileFrom, rankFrom);
	toBoard->placePiece(fileTo, rankTo, piece);
	toBoard->placePiece(fileFrom, rankFrom, EMPTY_CELL);
	
	

	ChessMove::ptr curMove(new ChessMove);
	
	curMove->previous=true;
	curMove->from=fromBoard;
	curMove->to=toBoard;
	/*
	curMove->notation=std::string("")+
		chessPieceStrings[piece]+" "+
		fileFrom+std::to_string(rankFrom)+" "+
		fileTo+std::to_string(rankTo);
		*/
	curMove->moveNum=fromBoard->getMove()->moveNum+1;
	toBoard->move=curMove;
	toBoard->turn=!fromBoard->getTurn();
	
	assert(fromBoard->getTurn()!=toBoard->getTurn());
	assert(fromBoard->getTurn()==curMove->getTurn());
	
	return toBoard;
}