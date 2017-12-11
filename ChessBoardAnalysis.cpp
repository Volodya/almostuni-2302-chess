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

inline constexpr weight_type domination(int8_t white, int8_t black)
{
	return
		white==black ? 0 :
		white>black ? 1 :
		-1;
}
constexpr weight_type weightFromPiece(const ChessPiece &cp)
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
	: board(std::move(board_)), possibleMoves(nullptr), check(false)
{
	assert(board!=nullptr);
	underAttackByBlack = new int8_t[size_t(board->getWidth())*board->getHeight()]{};
	underAttackByWhite = new int8_t[size_t(board->getWidth())*board->getHeight()]{};
	++constructed;
}

ChessBoardAnalysis::~ChessBoardAnalysis()
{
	delete[] underAttackByBlack;
	delete[] underAttackByWhite;
}

void ChessBoardAnalysis::calculatePossibleMoves()
{
	if(board->knownPossibleMoves != nullptr)
	{
		possibleMoves=board->knownPossibleMoves;
		check=board->knownCheck;
		return;
	}

	possibleMoves=new std::vector<ChessBoard::ptr>();

	ChessBoardFactory factory;
	typedef ChessMove::ChessMoveRecordingFunction ChessMoveRecordingFunction;
		// empty function
	const static ChessMoveRecordingFunction emptyFunction =
			[](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {};

	
		// take opponent's piece
		// or
		// attack empty space
	ChessMoveRecordingFunction functionTake[2][2] = 
	{
		// white turn
		{
			//white
			[this, &factory](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				assert(this->board->getTurn()==ChessPlayerColour::WHITE);
				
				auto nextBoard = factory.createBoard(this->board, pos, newPos);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::BLACK);
				assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				if(maybeMove->isMovePossible())
				{
					this->possibleMoves->push_back(nextBoard);
				}
				else
				{
					nextBoard->move.reset();
					nextBoard.reset();
				}
			},
			//black
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(this->board->getPiecePos(newPos)==KING_WHITE)
				{
					check=true;
				}
			}
		},
		// black turn
		{
			// white
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);
				if(this->board->getPiecePos(newPos)==KING_BLACK)
				{
					check=true;
				}
			},
			// black
			[this, &factory](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				assert(this->board->getTurn()==ChessPlayerColour::BLACK);

				auto nextBoard = factory.createBoard(this->board, pos, newPos);
				auto maybeMove = nextBoard->getMove();
				
				assert(nextBoard->getTurn()==ChessPlayerColour::WHITE);
				assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);
				
				if(maybeMove->isMovePossible())
				{
					this->possibleMoves->push_back(nextBoard);
				}
				else
				{
					nextBoard->move.reset();
					nextBoard.reset();
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
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByWhite[newPos];
			},
			// black
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByWhite[newPos];
			}
		},
		// black's turn
		{
			// white
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::WHITE);
				
				++underAttackByBlack[newPos];
			},
			// black
			[this](ChessBoard::BoardPosition_t pos, ChessBoard::BoardPosition_t newPos) {
				//assert(maybeMove->getTurn()==ChessPlayerColour::BLACK);

				++underAttackByBlack[newPos];
			}
		}
	};

	for(ChessBoard::BoardPosition_t pos=0, end=board->getCellCount(); pos!=end; ++pos)
	{
		auto curPiece = board->getPiecePos(pos);
		if(curPiece == EMPTY_CELL) continue;
		
		auto pieceParam = moveParameters.at(curPiece);
		auto moveArrayPos = toArrayPosition(board->getTurn());
		auto pieceArrayPos = toArrayPosition(getColour(curPiece));
		
		if(pieceParam->isDifferentMoveTypes)
		{
			ChessMove::moveAttempts(functionNoTake[moveArrayPos][pieceArrayPos], emptyFunction,
				*board, pos,
				*pieceParam->noTakeMove, false);
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
				*board, pos,
				*pieceParam->takeMove, true, false);
		}
		else
		{
			ChessMove::moveAttempts(functionTake[moveArrayPos][pieceArrayPos],
				functionDefend[moveArrayPos][pieceArrayPos],
				*board, pos, *pieceParam->anyMove, true);
		}
	}
	if(board->getTurn()==ChessPlayerColour::WHITE)
	{
		// process white pawns on first ranks
		for(ChessBoard::BoardPosition_t pos = 0, end=board->getWidth()*2; pos<end; ++pos)
		{
			auto curPiece = board->getPiecePos(pos);
			if(curPiece != PAWN_WHITE) continue;
			
			auto enPassan=pos+board->getWidth();
			if(board->getPiecePos(enPassan) == EMPTY_CELL)
			{
				auto newPos = pos+2*board->getWidth();
				if(board->getPiecePos(newPos)==EMPTY_CELL)
				{
					auto nextBoard = factory.createBoard(this->board, pos, newPos);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						nextBoard->enPassan=enPassan;
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
		}
		// process en passan rules
		if(board->enPassan!=board->cellCount)
		{
			// test left
			if(board->enPassan % board->width != 0)
			{
				auto pos = board->enPassan - board->width - 1;
				if(board->getPiecePos(pos)==PAWN_WHITE)
				{
					auto nextBoard = factory.createBoard(this->board, pos, board->enPassan);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						++underAttackByWhite[pos+1];
						nextBoard->placePiecePos(pos+1, EMPTY_CELL);
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
			// test right
			if(board->enPassan % board->width != board->width-1)
			{
				auto pos = board->enPassan - board->width + 1;
				if(board->getPiecePos(pos)==PAWN_WHITE)
				{
					auto nextBoard = factory.createBoard(this->board, pos, board->enPassan);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						++underAttackByWhite[pos-1];
						nextBoard->placePiecePos(pos-1, EMPTY_CELL);
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
		}
		// process castling rules
		if(board->whiteCastling[0]!=board->cellCount) // if can castle left
		{
			assert(board->getPiecePos(board->whiteCastling[0])==ROOK_WHITE);
			if(
				underAttackByBlack[board->whiteKingPos[0]] == 0 &&
				underAttackByBlack[board->whiteKingPos[0]-1] == 0)
			{
				if(board->whiteKingPos[1] >=2) // farther than 2 files from the edge
				{
					bool allEmpty = true;
					for(ChessBoard::BoardPosition_t cell = board->whiteCastling[0]+1; cell<board->whiteKingPos[0]; ++cell)
					{
						if(board->getPiecePos(cell)!=EMPTY_CELL)
						{
							allEmpty=false;
							break;
						}
					}
					if(allEmpty && underAttackByBlack[board->whiteKingPos[0]-2] == 0)
					{
						auto nextBoard = factory.createBoard(this->board, 
							board->whiteKingPos[0], board->whiteKingPos[0]-2, // move king
							board->whiteCastling[0], board->whiteKingPos[0]-1 // move rook
							);
						if(nextBoard->getMove()->isMovePossible())
						{
							this->possibleMoves->push_back(nextBoard);
						}
						else
						{
							nextBoard->move.reset();
							nextBoard.reset();
						}
					}
				}
				else
				{
					auto nextBoard = factory.createBoard(this->board, 
						board->whiteKingPos[0], board->whiteKingPos[0]-1, // move king
						board->whiteCastling[0], board->whiteKingPos[0] // move rook
						);
					// not checking if move is possible, because we know that the cell on the left isn't
					// being attacked. note, that it cannot be assumed in the normal castling
					this->possibleMoves->push_back(nextBoard);
				}
			}
		}
		if(board->whiteCastling[1]!=board->cellCount) // if can castle right
		{
			assert(board->getPiecePos(board->whiteCastling[1])==ROOK_WHITE);
			if(
				underAttackByBlack[board->whiteKingPos[0]] == 0 &&
				underAttackByBlack[board->whiteKingPos[0]+1] == 0)
			{
				if(board->whiteKingPos[1] < board->width-2) // farther than 2 files from the edge
				{
					bool allEmpty = true;
					for(ChessBoard::BoardPosition_t cell = board->whiteCastling[0]+1; cell<board->whiteKingPos[0]; ++cell)
					{
						if(board->getPiecePos(cell)!=EMPTY_CELL)
						{
							allEmpty=false;
							break;
						}
					}
					if(allEmpty &&underAttackByBlack[board->whiteKingPos[0]+2] == 0)
					{
						auto nextBoard = factory.createBoard(this->board, 
							board->whiteKingPos[0], board->whiteKingPos[0]+2, // move king
							board->whiteCastling[1], board->whiteKingPos[0]+1 // move rook
							);
						if(nextBoard->getMove()->isMovePossible())
						{
							this->possibleMoves->push_back(nextBoard);
						}
						else
						{
							nextBoard->move.reset();
							nextBoard.reset();
						}
					}
				}
				else
				{
					auto nextBoard = factory.createBoard(this->board, 
						board->whiteKingPos[0], board->whiteKingPos[0]+1, // move king
						board->whiteCastling[1], board->whiteKingPos[0] // move rook
						);
					// not checking if move is possible, because we know that the cell on the left isn't
					// being attacked. note, that it cannot be assumed in the normal castling
					this->possibleMoves->push_back(nextBoard);
				}
			}
		}
	}
	else // if ChessPlayerColour::Black
	{
		// process black pawns on first ranks
		for(ChessBoard::BoardPosition_t pos = board->getCellCount()-1, end=board->getCellCount() - (board->getWidth()*2) -1; pos>=end; --pos)
		{
			
			auto curPiece = board->getPiecePos(pos);
			if(curPiece != PAWN_BLACK) continue;
			
			auto enPassan=pos-board->getWidth();
			if(board->getPiecePos(enPassan) == EMPTY_CELL)
			{
				auto newPos = pos-2*board->getWidth();
				if(board->getPiecePos(newPos)==EMPTY_CELL)
				{
					auto nextBoard = factory.createBoard(this->board, pos, newPos);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						nextBoard->enPassan=enPassan;
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
		}
		if(board->enPassan!=board->cellCount)
		{
			// test left
			if(board->enPassan % board->width != 0)
			{
				auto pos = board->enPassan + board->width - 1;
				if(board->getPiecePos(pos)==PAWN_BLACK)
				{
					auto nextBoard = factory.createBoard(this->board, pos, board->enPassan);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						++underAttackByBlack[pos+1];
						nextBoard->placePiecePos(pos+1, EMPTY_CELL);
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
			// test right
			if(board->enPassan % board->width != board->width-1)
			{
				auto pos = board->enPassan + board->width + 1;

				if(board->getPiecePos(pos)==PAWN_BLACK)
				{
					auto nextBoard = factory.createBoard(this->board, pos, board->enPassan);
					auto maybeMove = nextBoard->getMove();
					
					if(maybeMove->isMovePossible())
					{
						++underAttackByBlack[pos-1];
						nextBoard->placePiecePos(pos-1, EMPTY_CELL);
						this->possibleMoves->push_back(nextBoard);
					}
					else
					{
						nextBoard->move.reset();
						nextBoard.reset();
					}
				}
			}
		}
		// process castling rules
		if(board->blackCastling[0]!=board->cellCount) // if can castle left
		{
			assert(board->getPiecePos(board->blackCastling[0])==ROOK_BLACK);
			if(
				underAttackByWhite[board->blackKingPos[0]] == 0 &&
				underAttackByWhite[board->blackKingPos[0]-1] == 0)
			{
				if(board->blackKingPos[1] >=2) // farther than 2 files from the edge
				{
					bool allEmpty = true;
					for(ChessBoard::BoardPosition_t cell = board->blackCastling[0]+1; cell<board->blackKingPos[0]; ++cell)
					{
						if(board->getPiecePos(cell)!=EMPTY_CELL)
						{
							allEmpty=false;
							break;
						}
					}
					if(allEmpty && underAttackByWhite[board->blackKingPos[0]-2] == 0)
					{
						auto nextBoard = factory.createBoard(this->board, 
							board->blackKingPos[0], board->blackKingPos[0]-2, // move king
							board->blackCastling[0], board->blackKingPos[0]-1 // move rook
							);
						if(nextBoard->getMove()->isMovePossible())
						{
							this->possibleMoves->push_back(nextBoard);
						}
						else
						{
							nextBoard->move.reset();
							nextBoard.reset();
						}
					}
				}
				else
				{
					auto nextBoard = factory.createBoard(this->board, 
						board->blackKingPos[0], board->blackKingPos[0]-1, // move king
						board->blackCastling[0], board->blackKingPos[0] // move rook
						);
					// not checking if move is possible, because we know that the cell on the left isn't
					// being attacked. note, that it cannot be assumed in the normal castling
					this->possibleMoves->push_back(nextBoard);
				}
			}
		}
		if(board->blackCastling[1]!=board->cellCount) // if can castle right
		{
			assert(board->getPiecePos(board->blackCastling[1])==ROOK_BLACK);
			if(
				underAttackByWhite[board->blackKingPos[0]] == 0 &&
				underAttackByWhite[board->blackKingPos[0]+1] == 0)
			{
				if(board->blackKingPos[1] < board->width-2) // farther than 2 files from the edge
				{
					bool allEmpty = true;
					for(ChessBoard::BoardPosition_t cell = board->blackCastling[0]+1; cell<board->blackKingPos[0]; ++cell)
					{
						if(board->getPiecePos(cell)!=EMPTY_CELL)
						{
							allEmpty=false;
							break;
						}
					}
					if(allEmpty && underAttackByWhite[board->blackKingPos[0]+2] == 0)
					{
						auto nextBoard = factory.createBoard(this->board, 
							board->blackKingPos[0], board->blackKingPos[0]+2, // move king
							board->blackCastling[1], board->blackKingPos[0]+1 // move rook
							);
						if(nextBoard->getMove()->isMovePossible())
						{
							this->possibleMoves->push_back(nextBoard);
						}
						else
						{
							nextBoard->move.reset();
							nextBoard.reset();
						}
					}
				}
				else
				{
					auto nextBoard = factory.createBoard(this->board, 
						board->blackKingPos[0], board->blackKingPos[0]+1, // move king
						board->blackCastling[1], board->blackKingPos[0] // move rook
						);
					// not checking if move is possible, because we know that the cell on the left isn't
					// being attacked. note, that it cannot be assumed in the normal castling
					this->possibleMoves->push_back(nextBoard);
				}
			}
		}
	}
	board->knownPossibleMoves = possibleMoves;
	board->knownCheck = check;
}

weight_type ChessBoardAnalysis::chessPositionWeight() const
{
	if(isCheckMate())
	{
		weight_type wIsCheckMate = getWeightMultiplier(board->getTurn()) * CHECKMATE_WEIGHT;
		weight_type wMoveNum = getWeightMultiplier(board->getTurn())*board->getMove()->getMoveNum();
		
		return wIsCheckMate + wMoveNum;
	}
	else
	{
		weight_type wChessPieces = this->chessPiecesWeight();	// count pieces
		weight_type wChessPieceAttacked = this->chessPieceAttackedWeight(); // count attacked pieces
		weight_type wChessCentreControl = this->chessCentreControlWeight(); // control of the centre of the board

/*	
	Log::info(std::string("wIsCheckMate: ")+std::to_string(wIsCheckMate));
	Log::info(std::string("wChessPieces: ")+std::to_string(wChessPieces));
	Log::info(std::string("wChessPieceAttacked: ")+std::to_string(wChessPieceAttacked));
	Log::info(std::string("wChessCentreControl: ")+std::to_string(wChessCentreControl));
	Log::info(std::string("wMoveNum: ")+std::to_string(wMoveNum));
*/
		return wChessPieces + wChessPieceAttacked + wChessCentreControl;
	}
}

weight_type ChessBoardAnalysis::chessPiecesWeight() const
{
	int count[KNOWN_CHESS_PIECE_COUNT] = { 0 };
	for(ChessBoard::BoardPosition_t pos=0, end=board->getCellCount(); pos!=end; ++pos)
	{
		++count[board->getPiecePos(pos)]; // ChessPiece is a numerical constant
	}
	
	return
		PIECE_PRESENT_MILTIPLIER * (
		BOARD_PAWN_WEIGHT   * (count[PAWN_WHITE]   - count[PAWN_BLACK]) +
		BOARD_KNIGHT_WEIGHT * (count[KNIGHT_WHITE] - count[KNIGHT_BLACK]) +
		BOARD_BISHOP_WEIGHT * (count[BISHOP_WHITE] - count[BISHOP_BLACK]) +
		BOARD_ROOK_WEIGHT   * (count[ROOK_WHITE]   - count[ROOK_BLACK]) +
		BOARD_QUEEN_WEIGHT  * (count[QUEEN_WHITE]  - count[QUEEN_BLACK])
		);

}
weight_type ChessBoardAnalysis::chessPieceAttackedWeight() const
{
	weight_type result = 0;
	
	ChessPiece curPiece;
	for(ChessBoard::BoardPosition_t pos=0, end=board->getCellCount(); pos!=end; ++pos)
	{
		// get non-empty cells
		curPiece = board->getPiecePos(pos);
		if(curPiece == EMPTY_CELL) continue;
		
		auto multiplierColour = getWeightMultiplier(getColour(curPiece));
		auto dominator = domination( // who has more attacks -1 (black); 0 (neutral); 1 (white)
			underAttackByWhite[pos],
			underAttackByBlack[pos]
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
	const static weight_type CELL_WEIGHT_MULTIPLIER = 300;
	const static weight_type CELL_WEIGHT[64]
	{
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3,
		2, 2, 7, 7, 7, 7, 2, 2,
		1, 4, 6, 8, 8, 6, 4, 1,
		1, 4, 6, 8, 8, 6, 4, 1,
		2, 2, 7, 7, 7, 7, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3
	};
	
	weight_type result = 0;
	for(ChessBoard::BoardPosition_t pos=0, end=board->getCellCount(); pos!=end; ++pos)
	{
		result +=
			domination(
				underAttackByWhite[pos],
				underAttackByBlack[pos]
				)
			* CELL_WEIGHT[pos]
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
	/*
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
	*/
	
	if(isCheck())
	{
		return possibleMoves->empty();
	}
	
	return false;
}


std::vector<ChessBoard::ptr> * const ChessBoardAnalysis::getPossibleMoves() const
{
	return possibleMoves;
}

ChessBoard::ptr ChessBoardAnalysis::getBoard() const
{
	return board;
}
