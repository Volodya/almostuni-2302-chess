#include "chessfunctions.hpp"

#include "moveTemplate.hpp"

#include <iostream>


void move(std::vector<ChessBoard> &result, const ChessBoard &cb, char file, int rank,
	const MoveTemplate& mt, bool canTake=true, bool canNotTake=true)
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
				if(canTake && !ChessFunctions::ownPiece(cb.getPiece(newFile, newRank), cb.getTurn()))
				{
					result.push_back(cb.move(file, rank, newFile, newRank));
				}
			}
			else // from: if(!cb.isEmpty(newFile, newRank))
			{
				if(canNotTake)
				{
					result.push_back(cb.move(file, rank, newFile, newRank));
				}
			}
		}
	}
}

// Regular function

std::vector<ChessBoard> ChessFunctions::getPossibleMoves(ChessBoard cb)
{
	std::vector<ChessBoard> result;
	
	PlayerColour turn = cb.getTurn();

	for(auto it = cb.begin(); it != cb.end(); ++it)
	{
		if(*it == ' ') continue;
		
		if(!ownPiece(*it, turn)) continue;
				
		int rank = it.getRank();
		char file = it.getFile();

		switch(*it)
		{
			case 'p':
				move(result, cb, file, rank, pawnWhiteMoveNotTake, false, true);
				move(result, cb, file, rank, pawnWhiteMoveTake,    true, false);
				break;
			case 'P':
				move(result, cb, file, rank, pawnBlackMoveNotTake, false, true);
				move(result, cb, file, rank, pawnBlackMoveTake,    true, false);
				break;
			case 'b':
				break;
		}
	}
	return result;
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
	return (cp>='a' && cp<='z' && turn == WHITE);
}