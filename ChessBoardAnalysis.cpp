/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardAnalysis.hpp"

void calculatePossibleMoves(ChessBoard::ptr obj)
{
	using ChessFunctions::MoveRecordingFunction;
	
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
			[obj](char file, int rank, char newFile, int newRank) {};
	
	if(obj->turn==WHITE)
	{
		whiteTurnFunctionTake =
			[obj](char file, int rank, char newFile, int newRank) {
				auto maybeMove = move(obj, file, rank, newFile, newRank);
				
				assert(maybeMove->getTurn()==BLACK);
				
				if(maybeMove->isPositionPossible())
				{
					obj->possibleMoves.push_back(maybeMove);
				}
			};
		whiteTurnFunctionNoTake =
			[obj](char file, int rank, char newFile, int newRank) {
				auto maybeMove = move(obj, file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					obj->possibleMoves.push_back(maybeMove);
				}
			};
		whiteTurnFunctionDefend =
			[obj](char file, int rank, char newFile, int newRank) {
				++obj->underAttackByWhite[newRank-1][newFile-'A'];
			};

		blackTurnFunctionTake =
			[obj](char file, int rank, char newFile, int newRank) {
				
				if(obj->board[newRank-1][newFile-'A']=='K')
				{
					obj->check=true;
				}
			};
		blackTurnFunctionNoTake = emptyFunction;
		blackTurnFunctionDefend =
			[obj](char file, int rank, char newFile, int newRank) {
				++obj->underAttackByBlack[newRank-1][newFile-'A'];
			};
	}
	else // if BLACK
	{
		blackTurnFunctionTake =
			[obj](char file, int rank, char newFile, int newRank) {
				auto maybeMove = move(obj, file, rank, newFile, newRank);

				assert(maybeMove->getTurn()==WHITE);
				
				if(maybeMove->isPositionPossible())
				{
					obj->possibleMoves.push_back(maybeMove);
				}
			};
		blackTurnFunctionNoTake =
			[obj](char file, int rank, char newFile, int newRank) {
				auto maybeMove = move(obj, file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					obj->possibleMoves.push_back(maybeMove);
				}
			};
		blackTurnFunctionDefend =
			[obj](char file, int rank, char newFile, int newRank) {
				++obj->underAttackByBlack[newRank-1][newFile-'A'];
			};

		whiteTurnFunctionTake =
			[obj](char file, int rank, char newFile, int newRank) {
				
				if(obj->board[newRank-1][newFile-'A']=='k')
				{
					obj->check=true;
				}
			};
		whiteTurnFunctionNoTake = emptyFunction;
		whiteTurnFunctionDefend =
			[obj](char file, int rank, char newFile, int newRank) {
				++obj->underAttackByWhite[newRank-1][newFile-'A'];
			};
	}
	
	for(auto it = obj->begin(); it != obj->end(); ++it)
	{
		if(*it == ' ') continue;
		
		//if(!ChessFunctions::ownPiece(*it, turn)) continue;
				
		int rank = it.getRank();
		char file = it.getFile();

		switch(*it)
		{
			case 'P':
				ChessFunctions::move(whiteTurnFunctionNoTake, emptyFunction,
					*obj, file, rank,
					pawnWhiteMoveNotTake, false);
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank,
					pawnWhiteMoveTake,    true, false);
				break;
			case 'p':
				ChessFunctions::move(blackTurnFunctionNoTake, emptyFunction,
					*obj, file, rank,
					pawnBlackMoveNotTake, false);
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank,
					pawnBlackMoveTake,    true, false);
				break;
			case 'R':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank, rookMove, true);
				break;
			case 'r':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank, rookMove, true);
				break;
			case 'N':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank, knightMove, true);
				break;
			case 'n':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank, knightMove, true);
				break;
			case 'B':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank, bishopMove, true);
				break;
			case 'b':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank, bishopMove, true);
				break;
			case 'Q':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank, queenMove, true);
				break;
			case 'q':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank, queenMove, true);
				break;
			case 'K':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*obj, file, rank, kingMove, true);
				break;
			case 'k':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*obj, file, rank, kingMove, true);
				break;
		}
	}
	obj->possibleMovesCalculated=true;
	obj->possibleMovesValid=true;
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
