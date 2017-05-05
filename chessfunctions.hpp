#ifndef CHESSFUNCTIONS__
#define CHESSFUNCTIONS__

#include <vector>
#include "ChessBoard.hpp"

namespace ChessFunctions
{
	std::vector<ChessBoard> getPossibleMoves(ChessBoard cb);
	bool ownPiece(ChessPiece cp, PlayerColour turn);
};

#endif