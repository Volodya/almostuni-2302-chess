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

#include <string>

#include "Log.hpp"

typedef ChessBoardAnalysis::weight_type weight_type;

// helper

constexpr weight_type domination(int8_t white, int8_t black)
{
	return
		white==black ? 0 :
		white>black ? 1 :
		-1;
}
constexpr weight_type weightFromPiece(const ChessPiece cp)
{
	return 
		cp==PAWN_WHITE   || cp==PAWN_BLACK   ? BOARD_PAWN_WEIGHT :
		cp==KNIGHT_WHITE || cp==KNIGHT_BLACK ? BOARD_KNIGHT_WEIGHT :
		cp==BISHOP_WHITE || cp==BISHOP_BLACK ? BOARD_BISHOP_WEIGHT :
		cp==ROOK_WHITE   || cp==ROOK_BLACK   ? BOARD_ROOK_WEIGHT :
		cp==QUEEN_WHITE  || cp==QUEEN_BLACK  ? BOARD_QUEEN_WEIGHT :
		cp==KING_WHITE   || cp==KING_BLACK   ? BOARD_KING_WEIGHT :
		0;
}

// static variables

unsigned long long ChessBoardAnalysis::constructed=0;


// class functions

ChessBoardAnalysis::ChessBoardAnalysis(ChessBoard::ptr board_)
	: board(board_)
{
	assert(board!=nullptr);
	underAttackByBlack = new int8_t[size_t(board->getWidth())*board->getHeight()];
	underAttackByWhite = new int8_t[size_t(board->getWidth())*board->getHeight()];
	++constructed;
}

ChessBoardAnalysis::~ChessBoardAnalysis()
{
	delete[] underAttackByBlack;
	delete[] underAttackByWhite;
}

void ChessBoardAnalysis::calculatePossibleMoves()
{
	ChessBoardFactory factory;
	typedef ChessMove::ChessMoveRecordingFunction ChessMoveRecordingFunction;
		// empty function
	const static ChessMoveRecordingFunction emptyFunction =
			[](size_t file, size_t rank, size_t newFile, size_t newRank) {};

	
		// take opponent's piece
		// or
		// attack empty space
	ChessMoveRecordingFunction functionTake[2][2] = 
	{
		// white turn
		{
			//white
			[this, &factory](size_t file, size_t rank, size_t newFile, size_t newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::WHITE);

				auto nextBoard = factory.createBoard(this->board, file, rank, newFile, newRank);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::BLACK);
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(nextBoard);
				}
			},
			//black
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(this->board->getPiecePos(newFile, newRank)==KING_BLACK)
				{
					check=true;
				}
			}
		},
		// black turn
		{
			// white
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);
								
				if(this->board->getPiecePos(newFile, newRank)==KING_WHITE)
				{
					check=true;
				}
			},
			// black
			[this, &factory](size_t file, size_t rank, size_t newFile, size_t newRank) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);

				auto nextBoard = factory.createBoard(this->board, file, rank, newFile, newRank);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::WHITE);
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(maybeMove->isMovePossible())
				{
					possibleMoves.push_back(nextBoard);
				}
			}
		}
	};
		// move to empty space
		// with no possibility of attack
		// (pawn move forward)
	ChessMoveRecordingFunction functionNoTake[2][2] = 
	{
		// white turn
		{
			// white
			functionTake[toArrayPosition(ChessPlayerColour::WHITE)][toArrayPosition(ChessPlayerColour::WHITE)],
			// black
			emptyFunction
		},
		// black turn
		{
			// white
			emptyFunction,
			// black
			functionTake[toArrayPosition(ChessPlayerColour::BLACK)][toArrayPosition(ChessPlayerColour::BLACK)]
		}
	};
	
		// we could recapture on this square	
	ChessMoveRecordingFunction functionDefend[2][2] = 
	{
		// white's turn
		{
			// white
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByWhite[this->board->getPos(newFile, newRank)];
			},
			// black
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByWhite[this->board->getPos(newFile, newRank)];
			}
		},
		// black's turn
		{
			// white
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByBlack[this->board->getPos(newFile, newRank)];
			},
			// black
			[this](size_t file, size_t rank, size_t newFile, size_t newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByBlack[this->board->getPos(newFile, newRank)];
			}
		}
	};

	for(auto it = board->begin(), end=board->end(); it != end; ++it)
	{
		if(*it == EMPTY_CELL) continue;
		
		// todo make size_t
		size_t rank = it.getRankPos();
		size_t file = it.getFilePos();

		auto pieceParam = moveParameters.at(*it);
		auto moveArrayPos = toArrayPosition(board->getTurn());
		auto pieceArrayPos = toArrayPosition(getColour(*it));
		if(pieceParam->isDifferentMoveTypes)
		{
			ChessMove::moveAttempts(functionNoTake[moveArrayPos][pieceArrayPos], emptyFunction,
				*board, file, rank,
				*pieceParam->noTakeMove, false);
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
				*board, file, rank,
				*pieceParam->takeMove, true, false);
		}
		else
		{
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
				*board, file, rank, *pieceParam->anyMove, true);
		}
	}
}

weight_type ChessBoardAnalysis::chessPositionWeight() const
{
	weight_type wIsCheckMate = (isCheckMate() ? getWeightMultiplier(board->getTurn()) * CHECKMATE_WEIGHT : 0);

	weight_type wChessPieces = this->chessPiecesWeight();	// count pieces

	weight_type wChessPieceAttacked = this->chessPieceAttackedWeight(); // count attacked pieces

	weight_type wChessCentreControl = this->chessCentreControlWeight(); // control of the centre of the board
	
	
	return
		wIsCheckMate
		+
		wChessPieces
		+
		wChessPieceAttacked
		+
		wChessCentreControl
		;
}

weight_type ChessBoardAnalysis::chessPiecesWeight() const
{
	int count[KNOWN_CHESS_PIECE_COUNT] = { 0 };
	for(auto it=board->begin(), end=board->end(); it!=end; ++it)
	{
		++count[*it]; // ChessPiece is a numerical constant
	}
	
	return
		BOARD_PAWN_WEIGHT   * (count[PAWN_WHITE]   - count[PAWN_BLACK]) +
		BOARD_KNIGHT_WEIGHT * (count[KNIGHT_WHITE] - count[KNIGHT_BLACK]) +
		BOARD_BISHOP_WEIGHT * (count[BISHOP_WHITE] - count[BISHOP_BLACK]) +
		BOARD_ROOK_WEIGHT   * (count[ROOK_WHITE]   - count[ROOK_BLACK]) +
		BOARD_QUEEN_WEIGHT  * (count[QUEEN_WHITE]  - count[QUEEN_BLACK]);

}
weight_type ChessBoardAnalysis::chessPieceAttackedWeight() const
{
	weight_type result = 0;
	
	ChessPiece curPiece;
	for(auto it=board->begin(), end=board->end(); it!=end; ++it)
	{
		curPiece = *it;
		if(curPiece == EMPTY_CELL) continue;
		
		auto multiplierColour = getWeightMultiplier(getColour(curPiece));
		auto dominator = domination( // who has more attacks -1 (black); 0 (neutral); 1 (white)
			underAttackByWhite[it.getPos()],
			underAttackByBlack[it.getPos()]
			);
		auto attackOrDefence = PIECE_ATTACK_MULTIPLIER;
		if(multiplierColour==dominator)
		{
			attackOrDefence = PIECE_DEFENCE_MUTIPLIER; // defending own piece
		}
			
		result += dominator * weightFromPiece(curPiece) * attackOrDefence;
	}
	
	return result;
}

weight_type ChessBoardAnalysis::chessCentreControlWeight() const
{
	const static weight_type CELL_WEIGHT_MULTIPLIER = 3000;
	const static weight_type CELL_WEIGHT[8][8]
	{
		{ 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 2, 2, 7, 7, 7, 7, 2, 2 },
		{ 1, 4, 6, 8, 8, 6, 4, 1 },
		{ 1, 4, 6, 8, 8, 6, 4, 1 },
		{ 2, 2, 7, 7, 7, 7, 2, 2 },
		{ 3, 3, 3, 3, 3, 3, 3, 3 },
		{ 3, 3, 3, 3, 3, 3, 3, 3 }
	};
	
	weight_type result = 0;
	for(auto it=board->begin(), end=board->end(); it!=end; ++it)
	{
		result +=
			domination(
				underAttackByWhite[it.getPos()],
				underAttackByBlack[it.getPos()]
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


std::vector<ChessBoard::ptr> ChessBoardAnalysis::getPossibleMoves() const
{
	return possibleMoves;
}

ChessBoard::ptr ChessBoardAnalysis::getBoard() const
{
	return board;
}

ChessBoardHash ChessBoardAnalysis::getBoardHash() const
{
	return this->board->getHash();
}