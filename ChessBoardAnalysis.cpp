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
		cp==PAWN_WHITE   || cp==PAWN_BLACK   ? BOARD_PAWN_WEIGHT :
		cp==KNIGHT_WHITE || cp==KNIGHT_BLACK ? BOARD_KNIGHT_WEIGHT :
		cp==BISHOP_WHITE || cp==BISHOP_BLACK ? BOARD_BISHOP_WEIGHT :
		cp==ROOK_WHITE   || cp==ROOK_BLACK   ? BOARD_ROOK_WEIGHT :
		cp==QUEEN_WHITE  || cp==QUEEN_BLACK  ? BOARD_QUEEN_WEIGHT :
		cp==KING_WHITE   || cp==KING_BLACK   ? BOARD_KING_WEIGHT :
		0;
}

// class functions

ChessBoardAnalysis::ChessBoardAnalysis(ChessBoard::ptr board_)
	: board(board_)
{
	assert(board!=nullptr);
	this->calculatePossibleMoves();
}

ChessBoardAnalysis::~ChessBoardAnalysis()
{}

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
	ChessMoveRecordingFunction functionTake[2][2] = 
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
					possibleMoves.push_back(nextBoard);
				}
			},
			//black
			[this](char file, int rank, char newFile, int newRank) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(this->board->getPiece(newFile, newRank)==KING_BLACK)
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
								
				if(this->board->getPiece(newFile, newRank)==KING_WHITE)
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
		// black's turn
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
	Log::ptr log = Log::getInstance();
	for(auto it = board->begin(); it != board->end(); ++it)
	{
		if(*it == EMPTY_CELL) continue;
				
		// todo make size_t
		int rank = it.getRank();
		char file = it.getFile();

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

double ChessBoardAnalysis::chessPositionWeight() const
{
//	Log::ptr log = Log::getInstance();
//	log->log(Log::INFO, board->toFEN());
	
	double wIsCheckMate = (isCheckMate() ? getWeightMultiplier(board->getTurn()) * CHECKMATE_WEIGHT : 0);
//	log->log(Log::INFO, "wIsCheckMate=" + std::to_string(wIsCheckMate));
	double wChessPieces = this->chessPiecesWeight();	// count pieces
//	log->log(Log::INFO, "wChessPieces=" + std::to_string(wChessPieces));
	double wChessPieceAttacked = this->chessPieceAttackedWeight(); // count attacked pieces
//	log->log(Log::INFO, "wChessPieceAttacked=" + std::to_string(wChessPieceAttacked));
	double wChessCentreControl = this->chessCentreControlWeight(); // control of the centre of the board
//	log->log(Log::INFO, "wChessCentreControl=" + std::to_string(wChessCentreControl));
	
	
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

double ChessBoardAnalysis::chessPiecesWeight() const
{
	int count[KNOWN_CHESS_PIECE_COUNT] = { 0 };
	for(auto it=board->begin(); it!=board->end(); ++it)
	{
		++count[*it];
	}
	
	// todo: rewrite as a loop, create a 'ChessPiece.hpp'
	return
		BOARD_PAWN_WEIGHT   * (count[PAWN_WHITE]   - count[PAWN_BLACK]) +
		BOARD_KNIGHT_WEIGHT * (count[KNIGHT_WHITE] - count[KNIGHT_BLACK]) +
		BOARD_BISHOP_WEIGHT * (count[BISHOP_WHITE] - count[BISHOP_BLACK]) +
		BOARD_ROOK_WEIGHT   * (count[ROOK_WHITE]   - count[ROOK_BLACK]) +
		BOARD_QUEEN_WEIGHT  * (count[QUEEN_WHITE]  - count[QUEEN_BLACK]);

}
double ChessBoardAnalysis::chessPieceAttackedWeight() const
{
	double result = 0;
		
	for(auto it=board->begin(); it!=board->end(); ++it)
	{
		if(*it == EMPTY_CELL) continue;
		
		auto multiplierColour = getWeightMultiplier(getColour(*it));
		auto dominator = domination( // who has more attacks -1 (black); 0 (neutral); 1 (white)
			underAttackByWhite[it.getRankPos()][it.getFilePos()],
			underAttackByBlack[it.getRankPos()][it.getFilePos()]
			);
		auto attackOrDefence = PIECE_ATTACK_MULTIPLIER;
		if(multiplierColour==dominator)
		{
			attackOrDefence = PIECE_DEFENCE_MUTIPLIER; // defending own piece
		}
			
		result += dominator * weightFromPiece(*it) * attackOrDefence;
	}
	
	return result;
}

double ChessBoardAnalysis::chessCentreControlWeight() const
{
	const static double CELL_WEIGHT_MULTIPLIER = 3.0;
	const static double CELL_WEIGHT[8][8]
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
	return this->getBoard()->getHash();
}