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

// helper

constexpr double domination(int8_t white, int8_t black)
{
	return
		white==black ? 0 :
		white>black ? 1 :
		-1;
}
constexpr double weightFromPiece(const ChessPiece cp)
{
	return 
		cp=='P' || cp=='p' ? BOARD_PAWN_WEIGHT :
		cp=='N' || cp=='n' ? BOARD_KNIGHT_WEIGHT :
		cp=='B' || cp=='b' ? BOARD_BISHOP_WEIGHT :
		cp=='R' || cp=='r' ? BOARD_ROOK_WEIGHT :
		cp=='Q' || cp=='q' ? BOARD_QUEEN_WEIGHT :
		cp=='K' || cp=='k' ? BOARD_KING_WEIGHT :
		0;
}

// class functions

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
	const ChessMoveRecordingFunction functionTake[2][2] = 
	{
		// white turn
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
		},
		// black turn
		{
			// white
			[this](char file, int rank, char newFile, int newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);
								
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
		}
	};
		// move to empty space
		// with no possibility of attack
		// (pawn move forward)
	const ChessMoveRecordingFunction functionNoTake[2][2] = 
	{
		{
			// white
			functionTake[toArrayPosition(ChessPlayerColour::WHITE)][toArrayPosition(ChessPlayerColour::WHITE)],
			// black
			emptyFunction
		},
	//const static ChessMoveRecordingFunction blackTurnFunctionNoTake[2] =
		{
			// white
			emptyFunction,
			// black
			functionTake[toArrayPosition(ChessPlayerColour::BLACK)][toArrayPosition(ChessPlayerColour::BLACK)]
		}
	};
	
		// we could recapture on this square	
	const ChessMoveRecordingFunction functionDefend[2][2] = 
	{
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
		},
	//const static ChessMoveRecordingFunction blackTurnFunctionDefend[2] = 
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
		}
	};
	
	for(auto it = board->begin(); it != board->end(); ++it)
	{
		if(*it == EMPTY_CELL) continue;
		
		int rank = it.getRank();
		char file = it.getFile();

		auto pieceParam = moveParameters.at(*it);
		auto moveArrayPos = toArrayPosition(board->getTurn());
		auto pieceArrayPos = toArrayPosition(getColour(*it));
		if(pieceParam.isDifferentMoveTypes)
		{
			ChessMove::moveAttempts(functionNoTake[moveArrayPos][pieceArrayPos], emptyFunction,
				*board, file, rank,
				*pieceParam.noTakeMove, false);
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
				*board, file, rank,
				*pieceParam.takeMove, true, false);
		}
		else
		{
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
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
		
		+
		
		this->chessPieceAttackedWeight() // count attacked pieces
		
		+
		
		this->chessCentreControlWeight() // control of the centre of the board
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
double ChessBoardAnalysis::chessPieceAttackedWeight() const
{
	double result = 0;
		
	for(auto it=board->begin(); it!=board->end(); ++it)
	{
		if(*it == EMPTY_CELL) continue;
		
		auto multiplierColour = getWeightMultiplier(getColour(*it));
		auto dominator = domination(
			underAttackByWhite[it.getRankPos()][it.getFilePos()],
			underAttackByBlack[it.getRankPos()][it.getFilePos()]
			);
		auto attackOrDefence = PIECE_ATTACK_MULTIPLIER;
		if(multiplierColour==dominator)
		{
			attackOrDefence = PIECE_DEFENCE_MUTIPLIER; // defending own piece
		}
			
		result += multiplierColour * dominator * weightFromPiece(*it) * attackOrDefence;
	}
	
	return result;
}

double ChessBoardAnalysis::chessCentreControlWeight() const
{
	const static double CELL_WEIGHT_MULTIPLIER = 5.0;
	const static double CELL_WEIGHT[8][8]
	{
		{ 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 6, 6, 6, 6, 6, 6, 6, 6 },
		{ 2, 2, 7, 7, 7, 7, 2, 2 },
		{ 1, 4, 6, 8, 8, 6, 4, 1 },
		{ 1, 4, 6, 8, 8, 6, 4, 1 },
		{ 2, 2, 7, 7, 7, 7, 2, 2 },
		{ 6, 6, 6, 6, 6, 6, 6, 6 },
		{ 3, 3, 3, 3, 3, 3, 3, 3 }
	};
	
	double result = 0;
	for(auto it=board->begin(); it!=board->end(); ++it)
	{
		result +=
			domination(
				underAttackByWhite[it.getRankPos()][it.getFilePos()],
				underAttackByBlack[it.getRankPos()][it.getFilePos()]
				)
			* CELL_WEIGHT[it.getRankPos()][it.getFilePos()]
			* CELL_WEIGHT_MULTIPLIER;
	}
	return result;
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