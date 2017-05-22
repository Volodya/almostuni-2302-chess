#include "chessfunctions.hpp"

#include <iostream>



void ChessFunctions::move(
	MoveRecordingFunction recFunTake,
	MoveRecordingFunction recFunDefend,
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
					if(!ChessFunctions::ownPiece(cb.getPiece(newFile, newRank), cb.getTurn()))
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

// todo: reimplement iterator for const
int ChessFunctions::countPieces(/*const*/ ChessBoard &cb, std::function<bool(ChessPiece)> test)
{
	int result=0;
	for(auto it = cb.begin(); it != cb.end(); ++it)
	{
		if(test(*it))
		{
			++result;
		}
	}
	return result;
}
int ChessFunctions::countPieces(/*const*/ ChessBoard &cb, ChessPiece cp)
{
	return countPieces(cb, [&cp](ChessPiece onBoard) { return cp==onBoard; });
}


bool ChessFunctions::ownPiece(ChessPiece cp, PlayerColour turn)
{
	return
		(cp>='a' && cp<='z' && turn == BLACK)
		||
		(cp>='A' && cp<='Z' && turn == WHITE);
}