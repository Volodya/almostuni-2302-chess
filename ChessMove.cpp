/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessMove.hpp"

#include <iostream> // temporary

ChessMove::ChessMove()
{}

bool ChessMove::isMovePossible() const
{
	if(ChessBoard::ptr pTo = to.lock())
	{
		ChessPiece kingPiece;
		ChessPiece attackersPieces[6];
		if(pTo->getTurn()==ChessPlayerColour::WHITE)
		{
			// if it's white to move, we are looking for a black king
			kingPiece = 'k';
			attackersPieces[0]='Q';
			attackersPieces[1]='R';
			attackersPieces[2]='B';
			attackersPieces[3]='N';
			attackersPieces[4]='P';
			attackersPieces[5]='K';
		}
		else
		{
			kingPiece = 'K';
			attackersPieces[0]='q';
			attackersPieces[1]='r';
			attackersPieces[2]='b';
			attackersPieces[3]='n';
			attackersPieces[4]='p';
			attackersPieces[5]='k';
		}
		size_t king[2]={};
		bool found=false;
		for(size_t rank=0; !found && rank<8; ++rank)
		{
			for(size_t file=0; file<8; ++file)
			{
				if(
					(pTo->getPiecePos(file, rank)==kingPiece)
				  )
				{
					found = true;
					king[0] = rank;
					king[1] = file;
					break;
				}
			}
		}

		for(size_t i=0; i<6; ++i)
		{
			const MoveTemplate* takeMove = moveParameters.at(attackersPieces[i]).takeMove;
			for(auto dir=takeMove->begin(); dir!=takeMove->end(); ++dir)
			{
				for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
				{
					auto piece = pTo->getPiece(king[1]-pos->second+'A', king[0]-pos->first+1);
					if(piece==attackersPieces[i])
					{
						return false;
					}
					else if(piece!=EMPTY_CELL)
					{
						break;
					}
				}
			}
		}
		
		return true;
	}
	else
	{
		return false;
	}
}

bool ChessMove::hasPrevious() const
{
	return previous;
}
ChessBoard::ptr ChessMove::getFrom() const
{
	if(ChessBoard::ptr pFrom = from.lock())
	{
		return pFrom;
	}
	else
	{
		return nullptr;
	}
}
ChessBoard::ptr ChessMove::getTo() const
{
	if(ChessBoard::ptr pTo = to.lock())
	{
		return pTo;
	}
	else
	{
		return nullptr;
	}
}
ChessPlayerColour ChessMove::getTurn() const
{
	return !getTo()->getTurn();
}

void ChessMove::moveAttempts(
	const ChessMoveRecordingFunction &recFunTake,
	const ChessMoveRecordingFunction &recFunDefend,
	const ChessBoard &cb, char file, int rank,
	const MoveTemplate& mt,
	bool canTake, bool canMoveToEmpty)
{
	char newFile;
	int newRank;
	for(auto direction = mt.begin(); direction != mt.end(); ++direction)
	{
		for(auto attempt = direction->begin(); attempt != direction->end(); ++attempt)
		{
			newFile = file + attempt->first;
			newRank = rank+attempt->second;
			if(newFile<'A' || newFile>'H' || newRank<1 || newRank >8)
			{
				break;
			}
			
			if(!cb.isEmpty(newFile, newRank))
			{
				if(canTake)
				{
					if(getColour(cb.getPiece(newFile, newRank)) != cb.getTurn())
					{
						recFunTake(file, rank, newFile, newRank);
					}
					recFunDefend(file, rank, newFile, newRank);
				}
				break; // stop if a cell isn't empty
			}
			else // if empty
			{
				if(canMoveToEmpty)
				{
					recFunTake(file, rank, newFile, newRank);
				}
				recFunDefend(file, rank, newFile, newRank);
			}
		}
	}
}
