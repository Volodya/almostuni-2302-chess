#include <iostream>
#include "ChessBoard.hpp"

#include "chessfunctions.hpp"

int main()
{
	ChessBoard cb;
	cb.placePiece('E', 2, 'p');
	cb.placePiece('D', 3, 'p');
	cb.placePiece('E', 4, 'P');
	cb.debugPrint();
	std::cout << "Weight: " << cb.weight() << std::endl;
	
	auto tmp = ChessFunctions::getPossibleMoves(cb);
	
	std::cout << "Number of possible moves: " << tmp.size() << std::endl;
	
	for(auto it=tmp.begin(); it!=tmp.end(); ++it)
	{
		it->debugPrint();
		std::cout << "Weight: " << it->weight() << std::endl;
	}
	return 0;
}