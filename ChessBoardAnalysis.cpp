/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardAnalysis.hpp"

ChessBoardAnalysis::ChessBoardAnalysis(ChessBoard::ptr board_)
	: board(board_)
{
	this->calculatePossibleMoves();
}

void ChessBoardAnalysis::calculatePossibleMoves()
{
	using ChessFunctions::MoveRecordingFunction;
	
	ChessBoardFactory factory;
	
		// take opponent's piece
		// or
		// attack empty space
	MoveRecordingFunction whiteTurnFunctionTake, blackTurnFunctionTake;
		// move to empty space
		// with no possibility of attack
		// (pawn move forward)
	MoveRecordingFunction whiteTurnFunctionNoTake, blackTurnFunctionNoTake;
		// we could recapture on this square	
	MoveRecordingFunction whiteTurnFunctionDefend, blackTurnFunctionDefend;
		// empty function
	MoveRecordingFunction emptyFunction =
			[board](char file, int rank, char newFile, int newRank) {};
	
	if(board->turn==WHITE)
	{
		whiteTurnFunctionTake =
			[board, &factory](char file, int rank, char newFile, int newRank) {
				auto maybeMove = factory.createBoard(board, file, rank, newFile, newRank)->getMove();
				
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(maybeMove);
				}
			};
		whiteTurnFunctionNoTake = whiteTurnFunctionTake;
		whiteTurnFunctionDefend =
			[](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByWhite[newRank-1][newFile-'A'];
			};

		blackTurnFunctionTake =
			[board](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
								
				if(board->getPiece(newFile, newRank)=='K')
				{
					check=true;
				}
			};
		blackTurnFunctionNoTake = emptyFunction;
		blackTurnFunctionDefend =
			[board](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByBlack[newRank-1][newFile-'A'];
			};
	}
	else // if BLACK
	{
		blackTurnFunctionTake =
			[board, &factory](char file, int rank, char newFile, int newRank) {
				auto maybeMove = factory.createBoard(board, file, rank, newFile, newRank)->getMove();

				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(maybeMove);
				}
			};
		blackTurnFunctionNoTake = blackTurnFunctionTake;
		blackTurnFunctionDefend =
			[](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByBlack[newRank-1][newFile-'A'];
			};

		whiteTurnFunctionTake =
			[board](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(board->getPiece(newFile, newRank)=='k')
				{
					check=true;
				}
			};
		whiteTurnFunctionNoTake = emptyFunction;
		whiteTurnFunctionDefend =
			[board](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByWhite[newRank-1][newFile-'A'];
			};
	}
	
	for(auto it = board->begin(); it != board->end(); ++it)
	{
		if(*it == ' ') continue;
		
		//if(!ChessFunctions::ownPiece(*it, turn)) continue;
				
		int rank = it.getRank();
		char file = it.getFile();

		switch(*it)
		{
			case 'P':
				ChessFunctions::move(whiteTurnFunctionNoTake, emptyFunction,
					*board, file, rank,
					pawnWhiteMoveNotTake, false);
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank,
					pawnWhiteMoveTake,    true, false);
				break;
			case 'p':
				ChessFunctions::move(blackTurnFunctionNoTake, emptyFunction,
					*board, file, rank,
					pawnBlackMoveNotTake, false);
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank,
					pawnBlackMoveTake,    true, false);
				break;
			case 'R':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank, rookMove, true);
				break;
			case 'r':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank, rookMove, true);
				break;
			case 'N':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank, knightMove, true);
				break;
			case 'n':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank, knightMove, true);
				break;
			case 'B':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank, bishopMove, true);
				break;
			case 'b':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank, bishopMove, true);
				break;
			case 'Q':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank, queenMove, true);
				break;
			case 'q':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank, queenMove, true);
				break;
			case 'K':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*board, file, rank, kingMove, true);
				break;
			case 'k':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*board, file, rank, kingMove, true);
				break;
		}
	}
}

double ChessBoardAnalysis::chessPositionWeight() const
{
	return
		(isCheckMate() ? (double)turn * CHECKMATE_WEIGHT : 0) // Check Mate

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

bool ChessBoard::isCheck() const
{
	assert(possibleMovesCalculated);
	return check;
}

std::vector<ChessBoard::ptr> ChessBoard::getPossibleMoves() const
{
	return possibleMoves;
}
