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
	ChessBoardFactory factory;
		// empty function
	const static ChessMoveRecordingFunction emptyFunction =
			[board](char file, int rank, char newFile, int newRank) {};

	
		// take opponent's piece
		// or
		// attack empty space
	const static ChessMoveRecordingFunction whiteTurnFunctionTake[2] = 
		{
			//white
			[board, &factory](char file, int rank, char newFile, int newRank) {
				auto maybeMove = factory.createBoard(board, file, rank, newFile, newRank)->getMove();
				
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(maybeMove);
				}
			},
			//black
			[board](char file, int rank, char newFile, int newRank) {
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(board->getPiece(newFile, newRank)=='k')
				{
					check=true;
				}
			}
		};
	const static ChessMoveRecordingFunction  blackTurnFunctionTake[2] =
		{
			// white
			[board](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
								
				if(board->getPiece(newFile, newRank)=='K')
				{
					check=true;
				}
			},
			// black
			[board, &factory](char file, int rank, char newFile, int newRank) {
				auto maybeMove = factory.createBoard(board, file, rank, newFile, newRank)->getMove();
				
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
			whiteTurnFunctionTake[getArrayPosition(ChessPlayerColour::WHITE)],
			// black
			emptyFunction
		};
	const static ChessMoveRecordingFunction blackTurnFunctionNoTake[2] =
		{
			// white
			emptyFunction,
			// black
			blackTurnFunctionTake[getArrayPosition(ChessPlayerColour::BLACK)]
		};
	
		// we could recapture on this square	
	const static ChessMoveRecordingFunction whiteTurnFunctionDefend[2] = 
		{
			// white
			[](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByWhite[newRank-1][newFile-'A'];
			},
			// black
			[](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByWhite[newRank-1][newFile-'A'];
			}
		};
	const static ChessMoveRecordingFunction blackTurnFunctionDefend[2] = 
		{
			// white
			[](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByBlack[newRank-1][newFile-'A'];
			},
			// black
			[](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByBlack[newRank-1][newFile-'A'];
			}
		};
	
	auto funcArrayPos = toArrayPosition(board->getTurn());
	for(auto it = board->begin(); it != board->end(); ++it)
	{
		if(*it == ' ') continue;
		
		//if(!ChessFunctions::ownPiece(*it, turn)) continue;
				
		int rank = it.getRank();
		char file = it.getFile();

		auto pieceParam = moveParameters[*it];
		if(pieceParam.isDifferentMoveTypes)
		{
			ChessFunctions::move(whiteTurnFunctionNoTake[funcArrayPos], emptyFunction,
				*board, file, rank,
				*pieceParam.noTakeMove, false);
			ChessFunctions::move(whiteTurnFunctionTake[funcArrayPos], whiteTurnFunctionDefend[funcArrayPos],
				*board, file, rank,
				*pieceParam.takeMove, true, false);
		}
		else
		{
			ChessFunctions::move(whiteTurnFunctionTake[funcArrayPos], whiteTurnFunctionDefend[funcArrayPos],
				*board, file, rank, *pieceParam.anyMove, true);
		}
		/*
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
		*/
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
