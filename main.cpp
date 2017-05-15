#include <iostream>
#include "ChessBoard.hpp"

#include "chessfunctions.hpp"

int main()
{
	ChessBoard cb;
	// standard chess board
	cb.placePiece('A', 2, 'p');
	cb.placePiece('B', 2, 'p');
	cb.placePiece('C', 2, 'p');
	cb.placePiece('D', 2, 'p');
	cb.placePiece('E', 2, 'p');
	cb.placePiece('F', 2, 'p');
	cb.placePiece('G', 2, 'p');
	cb.placePiece('H', 2, 'p');
	cb.placePiece('A', 1, 'r');
	cb.placePiece('B', 1, 'n');
	cb.placePiece('C', 1, 'b');
	cb.placePiece('D', 1, 'q');
	cb.placePiece('E', 1, 'k');
	cb.placePiece('F', 1, 'b');
	cb.placePiece('G', 1, 'n');
	cb.placePiece('H', 1, 'r');
	cb.placePiece('A', 7, 'P');
	cb.placePiece('B', 7, 'P');
	cb.placePiece('C', 7, 'P');
	cb.placePiece('D', 7, 'P');
	cb.placePiece('E', 7, 'P');
	cb.placePiece('F', 7, 'P');
	cb.placePiece('G', 7, 'P');
	cb.placePiece('H', 7, 'P');
	cb.placePiece('A', 8, 'R');
	cb.placePiece('B', 8, 'N');
	cb.placePiece('C', 8, 'B');
	cb.placePiece('D', 8, 'Q');
	cb.placePiece('E', 8, 'K');
	cb.placePiece('F', 8, 'B');
	cb.placePiece('G', 8, 'N');
	cb.placePiece('H', 8, 'R');
	
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