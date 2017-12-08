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

std::vector<std::weak_ptr<ChessBoard>> ChessBoardFactory::allBoards;

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

	allBoards.push_back(cb);

	return cb;
}

	// starting position: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
ChessBoard::ptr ChessBoardFactory::createBoard(std::string fen)
{
	ChessGameParameters::ptr param(new ChessGameParameters());
	param->setDimentions(8, 8);
	param->addPossiblePieces(STANDARD_GAME_PIECES);
	
	ChessBoard::ptr cb(new ChessBoard(param));
	
	bool hadBlackKing = false;
	bool hadWhiteKing = false;
	
	size_t file=0, rank=7;
	for(auto it=fen.begin(), end=fen.end(); it!=end; ++it)
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
			auto piece = charToChessPiece(*it);
			auto pos = cb->getPos(file, rank);
			cb->placePiecePos(pos, piece);
			
			// TODO: find how to realise this in FEN to make random chess work
			if(piece==ROOK_WHITE && rank==0)
			{
				cb->whiteCastling[ hadWhiteKing ? 1 : 0 ] = pos;
			}
			else if(piece==KING_WHITE)
			{
				hadWhiteKing = true;
				cb->whiteKingPos[0] = pos;
				cb->whiteKingPos[1] = pos % cb->width;
				cb->whiteKingPos[2] = pos / cb->width;
			}
			else if(piece==ROOK_BLACK && rank==7)
			{
				cb->blackCastling[ hadBlackKing ? 1 : 0 ] = pos;
			}
			else if(piece==KING_BLACK)
			{
				hadBlackKing = true;
				cb->blackKingPos[0] = pos;
				cb->blackKingPos[1] = pos % cb->width;
				cb->blackKingPos[2] = pos / cb->width;
			}
			++file;
		}
	}
	
	ChessMove::ptr cm(new ChessMove);
	
	cm->setTo(cb);
	cm->moveNum=0;
	cm->previous=false;
	cb->move=cm;
	
	allBoards.push_back(cb);
		
	return cb;
}

ChessBoard::ptr ChessBoardFactory::createBoard
  (ChessBoard::ptr fromBoard)
{
	ChessMove::ptr curMove(new ChessMove);
	
	curMove->previous=true;
	curMove->setFrom(fromBoard);

	ChessBoard::ptr toBoard(new ChessBoard(*fromBoard, curMove));
	
	toBoard->turn=!fromBoard->getTurn();
	
	curMove->setTo(toBoard);
	curMove->moveNum=fromBoard->getMove()->moveNum+1;
	toBoard->move=curMove;
	
	assert(fromBoard->getTurn()!=toBoard->getTurn());
	assert(fromBoard->getTurn()==curMove->getTurn());
	
	allBoards.push_back(toBoard);
	
	return toBoard;
}

ChessBoard::ptr ChessBoardFactory::createBoard
  (ChessBoard::ptr fromBoard, size_t posFrom, size_t posTo)
{
	ChessBoard::ptr toBoard = this->createBoard(fromBoard);
	
	// moving one of the pieces to the new position
	assert(posFrom!=posTo);
	auto piece = fromBoard->getPiecePos(posFrom);
	toBoard->placePiecePos(posFrom, EMPTY_CELL);
	toBoard->placePiecePos(posTo, piece);
	
	if(piece==KING_WHITE)
	{
		// change king's position
		toBoard->whiteKingPos[0]=posTo;
		toBoard->whiteKingPos[1]=posTo % toBoard->width;
		toBoard->whiteKingPos[2]=posTo / toBoard->width;
		
		// disallow castling both sides
		toBoard->whiteCastling[0] = toBoard->whiteCastling[1] = toBoard->cellCount;
	}
	else if(piece==KING_BLACK)
	{
		// change king's position
		toBoard->blackKingPos[0]=posTo;
		toBoard->blackKingPos[1]=posTo % toBoard->width;
		toBoard->blackKingPos[2]=posTo / toBoard->width;
		
		// disallow castling both sides
		toBoard->blackCastling[0] = toBoard->blackCastling[1] = toBoard->cellCount;
	}
	
	// if one of the rooks, that could castle, has moved, disallow castling with it
	// if one of the rooks, that could castle, was captured, disallow castling with it
	if(posFrom == toBoard->whiteCastling[0] || posTo == toBoard->whiteCastling[0])
	{
		toBoard->whiteCastling[0] = toBoard->cellCount;
	}
	else if(posFrom == toBoard->whiteCastling[1] || posTo == toBoard->whiteCastling[1]) 
	{
		toBoard->whiteCastling[1] = toBoard->cellCount;
	}
	else if(posFrom == toBoard->blackCastling[0] || posTo == toBoard->blackCastling[0])
	{
		toBoard->blackCastling[0] = toBoard->cellCount;
	}
	else if(posFrom == toBoard->blackCastling[1] || posTo == toBoard->blackCastling[1])
	{
		toBoard->blackCastling[1] = toBoard->cellCount;
	}
	
	return toBoard;
}

ChessBoard::ptr ChessBoardFactory::createBoard
  (ChessBoard::ptr fromBoard, size_t posFrom1, size_t posTo1, size_t posFrom2, size_t posTo2)
{
	ChessBoard::ptr toBoard = this->createBoard(fromBoard);
	
	// moving one of the pieces to the new position
	assert(posFrom1!=posTo1);
	assert(posFrom2!=posTo2);
	auto piece1 = fromBoard->getPiecePos(posFrom1);
	auto piece2 = fromBoard->getPiecePos(posFrom2);
	toBoard->placePiecePos(posFrom1, EMPTY_CELL);
	toBoard->placePiecePos(posFrom2, EMPTY_CELL);
	toBoard->placePiecePos(posTo1, piece1);
	toBoard->placePiecePos(posTo2, piece2);
	
	if(piece1==KING_WHITE)
	{
		// change king's position
		toBoard->whiteKingPos[0]=posTo1;
		toBoard->whiteKingPos[1]=posTo1 % toBoard->width;
		toBoard->whiteKingPos[2]=posTo1 / toBoard->width;
		
		// disallow castling both sides
		toBoard->whiteCastling[0] = toBoard->whiteCastling[1] = toBoard->cellCount;
	}
	else if(piece2==KING_WHITE)
	{
		// change king's position
		toBoard->whiteKingPos[0]=posTo2;
		toBoard->whiteKingPos[1]=posTo2 % toBoard->width;
		toBoard->whiteKingPos[2]=posTo2 / toBoard->width;
		
		// disallow castling both sides
		toBoard->whiteCastling[0] = toBoard->whiteCastling[1] = toBoard->cellCount;
	}
	else if(piece1==KING_BLACK)
	{
		// change king's position
		toBoard->blackKingPos[0]=posTo1;
		toBoard->blackKingPos[1]=posTo1 % toBoard->width;
		toBoard->blackKingPos[2]=posTo1 / toBoard->width;
		
		// disallow castling both sides
		toBoard->blackCastling[0] = toBoard->blackCastling[1] = toBoard->cellCount;
	}
	else if(piece2==KING_BLACK)
	{
		// change king's position
		toBoard->blackKingPos[0]=posTo2;
		toBoard->blackKingPos[1]=posTo2 % toBoard->width;
		toBoard->blackKingPos[2]=posTo2 / toBoard->width;
		
		// disallow castling both sides
		toBoard->blackCastling[0] = toBoard->blackCastling[1] = toBoard->cellCount;
	}

	return toBoard;
}