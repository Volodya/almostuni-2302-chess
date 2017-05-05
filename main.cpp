#include <iostream>
#include "ChessBoard.hpp"

#include "chessfunctions.hpp"

int main()
{
	ChessBoard cb;
	cb.debugPrint();
	cb.placePiece('E', 2, 'p');
	cb.debugPrint();
	
	auto tmp = ChessFunctions::getPossibleMoves(cb);
	
	std::cout << tmp.size();
	return 0;
}