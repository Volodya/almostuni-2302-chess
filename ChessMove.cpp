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
	return true;
	if(ChessBoard::ptr pTo = to.lock())
	{
		ChessPiece kingPiece;
		if(pTo->getTurn()==ChessPlayerColour::WHITE)
		{
			// if it's white to move, we are looking for a black king
			kingPiece = KING_BLACK;
		}
		else
		{
			kingPiece = KING_WHITE;
		}
		size_t king[2]={}; // TODO: change this monstrocity!!!!
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
		auto possiblePieces = pTo->param->getPossiblePieces();
		for(auto attackingPiece = possiblePieces.begin(); attackingPiece != possiblePieces.end(); ++attackingPiece)
		{
			if(*attackingPiece == EMPTY_CELL || getColour(*attackingPiece) == pTo->getTurn())
			{
				continue;
			}
			const MoveTemplate* takeMove = moveParameters.at(*attackingPiece).takeMove;
			for(auto dir=takeMove->begin(); dir!=takeMove->end(); ++dir)
			{
				for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
				{
					int rank = king[0] - pos->first;
					int file = king[1] - pos->second;
					if(rank < 0 || file < 0 || rank >= 8 || file >= 8 ) // TODO: change to board dimentions
					{
						break;
					}
					auto piece = pTo->getPiecePos(file, rank);
					
					if(piece==*attackingPiece)
					{
						return false;
					}

					if(piece!=EMPTY_CELL)
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
	return from;
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
std::string ChessMove::getNotation() const
{
	return notation;
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
