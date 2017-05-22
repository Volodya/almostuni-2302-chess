#include "ChessBoard.hpp"
#include <iostream>

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
std::shared_ptr<ChessBoard> ChessBoard::move(char fileFrom, int rankFrom, char fileTo, int rankTo) const
{
	auto result = std::make_shared<ChessBoard>(*this);
	
	result->placePiece(fileTo, rankTo, this->getPiece(fileFrom, rankFrom));
	result->placePiece(fileFrom, rankFrom, ' ');
	
	result->turn = (this->turn==WHITE) ? BLACK : WHITE;
	
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

bool ChessBoard::isCheckMate() /*const*/
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
		if(!possibleMovesCalculated)
		{
			calculatePossibleMoves();
		}
		std::cout << possibleMoves.size() << std::endl;
		return possibleMoves.empty();
	}
	
	return false;
}

bool ChessBoard::isCheck()
{
	if(!possibleMovesCalculated)
	{
		calculatePossibleMoves();
	}
	
	return check;
	
	/*
	for(int rank=0; rank<8; ++rank)
	{
		for(int file=0; file<8; ++file)
		{
			if(board[rank][file]=='K' && turn==WHITE)
			{
				return underAttackByBlack[rank][file]>0;
			}
			if(board[rank][file]=='k' && turn==BLACK)
			{
				return underAttackByWhite[rank][file]>0;
			}
		}
	}
	return false;
	*/
}

void ChessBoard::calculatePossibleMoves()
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
			[this](char file, int rank, char newFile, int newRank) {};
	
	if(turn==WHITE)
	{
		whiteTurnFunctionTake =
			[this](char file, int rank, char newFile, int newRank) {
				auto maybeMove = this->move(file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					this->possibleMoves.push_back(maybeMove);
				}
			};
		whiteTurnFunctionNoTake =
			[this](char file, int rank, char newFile, int newRank) {
				auto maybeMove = this->move(file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					this->possibleMoves.push_back(maybeMove);
				}
			};
		whiteTurnFunctionDefend =
			[this](char file, int rank, char newFile, int newRank) {
				++this->underAttackByWhite[newRank-1][newFile-'A'];
			};

		blackTurnFunctionTake =
			[this](char file, int rank, char newFile, int newRank) {
				
				if(this->board[newRank-1][newFile-'A']=='K')
				{
					this->check=true;
				}
			};
		blackTurnFunctionNoTake = emptyFunction;
		blackTurnFunctionDefend =
			[this](char file, int rank, char newFile, int newRank) {
				++this->underAttackByBlack[newRank-1][newFile-'A'];
			};
	}
	else // if BLACK
	{
		blackTurnFunctionTake =
			[this](char file, int rank, char newFile, int newRank) {
				auto maybeMove = this->move(file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					this->possibleMoves.push_back(maybeMove);
				}
			};
		blackTurnFunctionNoTake =
			[this](char file, int rank, char newFile, int newRank) {
				auto maybeMove = this->move(file, rank, newFile, newRank);
				if(maybeMove->isPositionPossible())
				{
					this->possibleMoves.push_back(maybeMove);
				}
			};
		blackTurnFunctionDefend =
			[this](char file, int rank, char newFile, int newRank) {
				++this->underAttackByBlack[newRank-1][newFile-'A'];
			};

		whiteTurnFunctionTake =
			[this](char file, int rank, char newFile, int newRank) {
				
				if(this->board[newRank-1][newFile-'A']=='k')
				{
					this->check=true;
				}
			};
		whiteTurnFunctionNoTake = emptyFunction;
		whiteTurnFunctionDefend =
			[this](char file, int rank, char newFile, int newRank) {
				++this->underAttackByWhite[newRank-1][newFile-'A'];
			};
	}
	
	for(auto it = this->begin(); it != this->end(); ++it)
	{
		if(*it == ' ') continue;
		
		//if(!ChessFunctions::ownPiece(*it, turn)) continue;
				
		int rank = it.getRank();
		char file = it.getFile();

		switch(*it)
		{
			case 'P':
				ChessFunctions::move(whiteTurnFunctionNoTake, emptyFunction,
					*this, file, rank,
					pawnWhiteMoveNotTake, false);
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank,
					pawnWhiteMoveTake,    true, false);
				break;
			case 'p':
				ChessFunctions::move(blackTurnFunctionNoTake, emptyFunction,
					*this, file, rank,
					pawnBlackMoveNotTake, false);
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank,
					pawnBlackMoveTake,    true, false);
				break;
			case 'R':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank, rookMove, true);
				break;
			case 'r':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank, rookMove, true);
				break;
			case 'N':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank, knightMove, true);
				break;
			case 'n':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank, knightMove, true);
				break;
			case 'B':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank, bishopMove, true);
				break;
			case 'b':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank, bishopMove, true);
				break;
			case 'Q':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank, queenMove, true);
				break;
			case 'q':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank, queenMove, true);
				break;
			case 'K':
				ChessFunctions::move(whiteTurnFunctionTake, whiteTurnFunctionDefend,
					*this, file, rank, kingMove, true);
				break;
			case 'k':
				ChessFunctions::move(blackTurnFunctionTake, blackTurnFunctionDefend,
					*this, file, rank, kingMove, true);
				break;
		}
	}
	possibleMovesCalculated=true;
}

std::vector<std::shared_ptr<ChessBoard>> ChessBoard::getPossibleMoves() const
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