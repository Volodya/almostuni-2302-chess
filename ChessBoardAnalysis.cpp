/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardAnalysis.hpp"
#include "ChessBoardFactory.hpp"
#include "ChessBoardIterator.hpp"
#include "ChessPlayerColour.hpp"
#include <cassert>
#include <algorithm>

ChessBoardAnalysis::ChessBoardAnalysis(ChessBoard::ptr board_)
	: board(board_)
{
	this->calculatePossibleMoves();
}

void ChessBoardAnalysis::calculatePossibleMoves()
{
	ChessBoardFactory factory;
	typedef ChessMove::ChessMoveRecordingFunction ChessMoveRecordingFunction;
		// empty function
	const static ChessMoveRecordingFunction emptyFunction =
			[](char file, int rank, char newFile, int newRank) {};

	
		// take opponent's piece
		// or
		// attack empty space
	const static ChessMoveRecordingFunction whiteTurnFunctionTake[2] = 
		{
			//white
			[this, &factory](char file, int rank, char newFile, int newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::WHITE);

				auto nextBoard = factory.createBoard(this->board, file, rank, newFile, newRank);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::BLACK);
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(maybeMove);
				}
			},
			//black
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(this->board->getPiece(newFile, newRank)=='k')
				{
					check=true;
				}
			}
		};
	const static ChessMoveRecordingFunction  blackTurnFunctionTake[2] =
		{
			// white
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
								
				if(this->board->getPiece(newFile, newRank)=='K')
				{
					check=true;
				}
			},
			// black
			[this, &factory](char file, int rank, char newFile, int newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);

				auto nextBoard = factory.createBoard(this->board, file, rank, newFile, newRank);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::WHITE);
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(maybeMove);
				}
			}
		};
	
		// move to empty space
		// with no possibility of attack
		// (pawn move forward)
	const static ChessMoveRecordingFunction whiteTurnFunctionNoTake[2] = 
		{
			// white
			whiteTurnFunctionTake[toArrayPosition(ChessPlayerColour::WHITE)],
			// black
			emptyFunction
		};
	const static ChessMoveRecordingFunction blackTurnFunctionNoTake[2] =
		{
			// white
			emptyFunction,
			// black
			blackTurnFunctionTake[toArrayPosition(ChessPlayerColour::BLACK)]
		};
	
		// we could recapture on this square	
	const static ChessMoveRecordingFunction whiteTurnFunctionDefend[2] = 
		{
			// white
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByWhite[newRank-1][newFile-'A'];
			},
			// black
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByWhite[newRank-1][newFile-'A'];
			}
		};
	const static ChessMoveRecordingFunction blackTurnFunctionDefend[2] = 
		{
			// white
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByBlack[newRank-1][newFile-'A'];
			},
			// black
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByBlack[newRank-1][newFile-'A'];
			}
		};
	
	for(auto it = board->begin(); it != board->end(); ++it)
	{
		if(*it == EMPTY_CELL) continue;
		
		int rank = it.getRank();
		char file = it.getFile();

		auto pieceParam = moveParameters.at(*it);
		if(pieceParam.isDifferentMoveTypes)
		{
			ChessMove::moveAttempts(whiteTurnFunctionNoTake[funcArrayPos], emptyFunction,
				*board, file, rank,
				*pieceParam.noTakeMove, false);
			ChessMove::moveAttempts(whiteTurnFunctionTake[funcArrayPos], whiteTurnFunctionDefend[funcArrayPos],
				*board, file, rank,
				*pieceParam.takeMove, true, false);
		}
		else
		{
			ChessMove::moveAttempts(whiteTurnFunctionTake[funcArrayPos], whiteTurnFunctionDefend[funcArrayPos],
				*board, file, rank, *pieceParam.anyMove, true);
		}
	}
}

double ChessBoardAnalysis::chessPositionWeight() const
{
	return
		(isCheckMate() ? getWeightMultiplier(board->getTurn()) * CHECKMATE_WEIGHT : 0) // Check Mate

		+
		this->chessPiecesWeight()	// count pieces
		
		/*
			// todo: create more metrics
		+
		
			// count attacked pieces
			
		+
		
			// count attacked empty positions
		
		*/
		;
}

double ChessBoardAnalysis::chessPiecesWeight() const
{
	std::map<ChessPiece, int> count;
	for(auto it=board->begin(); it!=board->end(); ++it)
	{
		++count[*it];
	}
	
	// todo: rewrite as a loop, create a 'ChessPiece.hpp'
	return
		BOARD_PAWN_WEIGHT * (count['P'] - count['p']) +
		BOARD_KNIGHT_WEIGHT * (count['N'] - count['n']) +
		BOARD_BISHOP_WEIGHT * (count['B'] - count['b']) +
		BOARD_ROOK_WEIGHT * (count['R'] - count['r']) +
		BOARD_QUEEN_WEIGHT * (count['Q'] - count['q']);

}

bool ChessBoardAnalysis::isCheck() const
{
	return check;
}

bool ChessBoardAnalysis::isCheckMate() const
{
	int count=0;
	if(board->getTurn()==ChessPlayerColour::BLACK)
	{
		// count white pieces
		count=std::count_if(board->begin(), board->end(), [](ChessPiece onBoard) {
			return getColour(onBoard)==ChessPlayerColour::WHITE;
		});
	}
	else
	{
		// count black pieces
		count=std::count_if(board->begin(), board->end(), [](ChessPiece onBoard) {
			return getColour(onBoard)==ChessPlayerColour::BLACK;
		});
	}
	
	if(count==0)
	{
		return true;
	}
	
	if(isCheck())
	{
		return possibleMoves.empty();
	}
	
	return false;
}


std::vector<ChessMove::ptr> ChessBoardAnalysis::getPossibleMoves() const
{
	return possibleMoves;
}

ChessBoard::ptr ChessBoardAnalysis::getBoard() const
{
	return board;
}