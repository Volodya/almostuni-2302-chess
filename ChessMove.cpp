/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessMove.hpp"

ChessMove::ChessMove()
{}

/*
ChessMove::ChessMove(
	ChessBoard::ptr from_, char fileFrom, int rankFrom, char fileTo, int rankTo)
{
	to = ChessBoard::ptr(*from_);
	
	to->placePiece(fileTo, rankTo, from_->getPiece(fileFrom, rankFrom));
	to->placePiece(fileFrom, rankFrom, ' ');
	
	to->turn = (from->turn_==WHITE) ? BLACK : WHITE;
	to->from = *this;
}
*/

bool ChessMove::isMovePossible() const
{
	int king[2]={};
	bool found=false;
	for(int rank=0; !found && rank<8; ++rank)
	{
		for(int file=0; file<8; ++file)
		{
			// if it's white to move, we are looking for a black king
			if(
				(to->getPiece(file+'A', rank+1)=='K' && to->getTurn()==ChessPlayerColour::BLACK) || 
				(to->getPiece(file+'A', rank+1)=='k' && to->getTurn()==ChessPlayerColour::WHITE)
			  )
			{
				found = true;
				king[0] = rank;
				king[1] = file;
				break;
			}
		}
	}

	if(to->getTurn()==ChessPlayerColour::BLACK)
	{
		// checking pawns
		for(auto dir=pawnBlackMoveTake.begin(); dir!=pawnBlackMoveTake.end(); ++dir)
		{
			for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
			{
				auto piece = to->getPiece(king[1]-pos->second+'A', king[0]-pos->first+1);
				if(piece=='p')
				{
					return false;
				}
				else if(piece!=' ')
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
				auto piece = to->getPiece(king[1]-pos->second+'A', king[0]-pos->first+1);
				if(piece=='q')
				{
					return false;
				}
				else if(piece!=' ')
				{
					break;
				}
			}
		}
	}
	
	return true;
}

bool ChessMove::hasPrevious() const
{
	return (bool)from;
}
ChessBoard::ptr ChessMove::getFrom() const
{
	return from;
}
ChessBoard::ptr ChessMove::getTo() const
{
	return to;
}
ChessPlayerColour ChessMove::getTurn() const
{
	return !to->getTurn();
}

void ChessMove::moveAttempts(
	ChessMoveRecordingFunction recFunTake,
	ChessMoveRecordingFunction recFunDefend,
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
