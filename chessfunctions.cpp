#include "chessfunctions.hpp"

std::vector<ChessBoard> ChessFunctions::getPossibleMoves(ChessBoard cb)
{
	std::vector<ChessBoard> result;
	
	PlayerColour turn = cb.getTurn();

	for(auto it = cb.begin(); it != cb.end(); ++it)
	{
		if(*it == ' ') continue;
		
		if(!ownPiece(*it, turn)) continue;
		
		int rank = it->getRank();
		char file = it->getFile();

		switch(*it)
		{
			case 'p':
				if(rank<8) // То можно вперёд
				{
					if(cb.isEmpty(file, rank+1)
					{
						result.push_back(cb.move(file, rank, file, rank+1));
					}
				}
				
			case 'P':
		}
	}

}

bool ChessFunctions::ownPiece(ChessPiece cp, PlayerColour turn)
{
	return (cp>='a' && cp<='z' && turn == WHITE);
}