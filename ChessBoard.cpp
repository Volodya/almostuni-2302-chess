#include "ChessBoard.hpp"
#include <iostream>
#include <cassert>

#include "chessfunctions.hpp"
#include "moveTemplate.hpp"

ChessBoard::ChessBoard()
	: possibleMoves(0), possibleMovesCalculated(false), turn(WHITE), check(false)
{
	for(int i=0; i<8; ++i)
	{
		for(int j=0; j<8; ++j)
		{
			board[i][j] = ' ';
			underAttackByWhite[i][j]=0;
			underAttackByBlack[i][j]=0;
		}
	}
}

void ChessBoard::debugPrint() const
{
	std::cout << ' ';
	for(int i=0; i<8; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	for(int rank=8; rank>0; --rank)
	{
		std::cout << rank;
		for(int file=0; file<8; ++file)
		{
			std::cout << ' ' << board[rank-1][file];
		}
		std::cout << std::endl;
	}/*
	if(possibleMovesCalculated)
	{
		std::cout << "Under attack by white" << std::endl;
		std::cout << ' ';
		for(int i=0; i<8; ++i)
		{
			std::cout << ' ' << (char)('A'+i);
		}
		std::cout << std::endl;
		for(int rank=8; rank>0; --rank)
		{
			std::cout << rank;
			for(int file=0; file<8; ++file)
			{
				std::cout << ' ' << (int)underAttackByWhite[rank-1][file];
			}
			std::cout << std::endl;
		}
		std::cout << "Under attack by black" << std::endl;
		std::cout << ' ';
		for(int i=0; i<8; ++i)
		{
			std::cout << ' ' << (char)('A'+i);
		}
		std::cout << std::endl;
		for(int rank=8; rank>0; --rank)
		{
			std::cout << rank;
			for(int file=0; file<8; ++file)
			{
				std::cout << ' ' << (int)underAttackByBlack[rank-1][file];
			}
			std::cout << std::endl;
		}
	}*/
}

PlayerColour ChessBoard::getTurn() const
{
	return turn;
}

void ChessBoard::placePiece(char file, int rank, ChessPiece piece)
{
	board[rank-1][file-'A'] = piece;
}
ChessBoard::ptr move(
	ChessBoard::ptr from, char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	auto result = std::make_shared<ChessBoard>(*from);
	
	result->placePiece(fileTo, rankTo, from->getPiece(fileFrom, rankFrom));
	result->placePiece(fileFrom, rankFrom, ' ');
	
	result->turn = (from->turn==WHITE) ? BLACK : WHITE;
	result->from = from;
	
	return result;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return board[rank-1][file-'A'];
}
bool ChessBoard::isEmpty(char file, int rank) const
{
	return (board[rank-1][file-'A'] == ' ');
}

double ChessBoard::weight() /*const*/
{
	return
		(isCheckMate() ? (double)turn * CHECKMATE_WEIGHT : 0) // Check Mate

		+
			// count pieces
		(ChessFunctions::countPieces(*this, 'P') - ChessFunctions::countPieces(*this, 'p')) * BOARD_PAWN_WEIGHT
		+
		(ChessFunctions::countPieces(*this, 'K') - ChessFunctions::countPieces(*this, 'k')) * BOARD_KNIGHT_WEIGHT
		+
		(ChessFunctions::countPieces(*this, 'B') - ChessFunctions::countPieces(*this, 'b')) * BOARD_BISHOP_WEIGHT
		+
		(ChessFunctions::countPieces(*this, 'R') - ChessFunctions::countPieces(*this, 'r')) * BOARD_ROOK_WEIGHT
		+
		(ChessFunctions::countPieces(*this, 'Q') - ChessFunctions::countPieces(*this, 'q')) * BOARD_QUEEN_WEIGHT
		/*
			// todo: create more metrics
		+
		
			// count attacked pieces
			
		+
		
			// count attacked empty positions
		
		*/
		;
}

bool ChessBoard::isCheckMate() const
{
	int count=0;
	if(turn==BLACK)
	{
		// count white pieces
		count=ChessFunctions::countPieces(*this, [](ChessPiece onBoard) {
			return onBoard=='p' || onBoard=='n' || onBoard=='b' || onBoard=='r' || onBoard=='q' || onBoard=='k';
		});
	}
	else
	{
		// count black pieces
		count=ChessFunctions::countPieces(*this, [](ChessPiece onBoard) {
			return onBoard=='P' || onBoard=='N' || onBoard=='B' || onBoard=='R' || onBoard=='Q' || onBoard=='K';
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

bool ChessBoard::isCheck() const
{
	assert(possibleMovesCalculated);
	return check;
}

void calculatePossibleMoves(ChessBoard::ptr obj)
{
	if(obj->possibleMovesCalculated) return;
	
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
}

std::vector<ChessBoard::ptr> ChessBoard::getPossibleMoves() const
{
	return possibleMoves;
}

bool ChessBoard::isPositionPossible() const
{
	int king[2]={};
	bool found=false;
	for(int rank=0; !found && rank<8; ++rank)
	{
		for(int file=0; file<8; ++file)
		{
			// if it's white to move, we are looking for a black king
			if(
				(board[rank][file]=='K' && turn==BLACK) || 
				(board[rank][file]=='k' && turn==WHITE)
			  )
			{
				found = true;
				king[0] = rank;
				king[1] = file;
				break;
			}
		}
	}

	if(turn==BLACK)
	{
		// checking pawns
		for(auto dir=pawnBlackMoveTake.begin(); dir!=pawnBlackMoveTake.end(); ++dir)
		{
			for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
			{
				if(board[king[0]-pos->first][king[1]-pos->second]=='p')
				{
					return false;
				}
				else if(board[king[0]-pos->first][king[1]-pos->second]!=' ')
				{
					break;
				}
			}
		}
		// checking queen
		for(auto dir=queenMove.begin(); dir!=queenMove.end(); ++dir)
		{
			for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
			{
				if(board[king[0]-pos->first][king[1]-pos->second]=='q')
				{
					return false;
				}
				else if(board[king[0]-pos->first][king[1]-pos->second]!=' ')
				{
					break;
				}
			}
		}
	}
	
	return true;
}

// ITERATOR

ChessBoardIterator ChessBoard::begin()
{
	return ChessBoardIterator(this, 0, 0);
}

ChessBoardIterator ChessBoard::end()
{
	return ChessBoardIterator(this, 8, 0);
}

ChessBoardIterator::ChessBoardIterator(ChessBoard* cb_, int rank_, int file_)
	: rank(rank_), file(file_), cb(cb_)
{}

bool ChessBoardIterator::operator!=(const ChessBoardIterator& that)
{
	return !operator==(that);
}
bool ChessBoardIterator::operator==(const ChessBoardIterator& that)
{
	return this->rank==that.rank && this->file==that.file;
}
ChessBoardIterator::value_type ChessBoardIterator::operator*() const
{
	return cb->board[rank][file];
}
ChessBoardIterator& ChessBoardIterator::operator++()
{
	++file;
	if(file>=8)
	{
		++rank;
		file=0;
	}
	return *this;
}

int ChessBoardIterator::getRank() const
{
	return rank+1;
}
char ChessBoardIterator::getFile() const
{
	return file+'A';
}


// CONST ITERATOR

ChessBoardConstIterator ChessBoard::begin() const
{
	return ChessBoardConstIterator(this, 0, 0);
}

ChessBoardConstIterator ChessBoard::end() const
{
	return ChessBoardConstIterator(this, 8, 0);
}

ChessBoardConstIterator::ChessBoardConstIterator(const ChessBoard* cb_, int rank_, int file_)
	: rank(rank_), file(file_), cb(cb_)
{}

bool ChessBoardConstIterator::operator!=(const ChessBoardConstIterator& that)
{
	return !operator==(that);
}
bool ChessBoardConstIterator::operator==(const ChessBoardConstIterator& that)
{
	return this->rank==that.rank && this->file==that.file;
}
ChessBoardConstIterator::value_type ChessBoardConstIterator::operator*() const
{
	return cb->board[rank][file];
}
ChessBoardConstIterator& ChessBoardConstIterator::operator++()
{
	++file;
	if(file>=8)
	{
		++rank;
		file=0;
	}
	return *this;
}

int ChessBoardConstIterator::getRank() const
{
	return rank+1;
}
char ChessBoardConstIterator::getFile() const
{
	return file+'A';
}