#ifndef CHESSFUNCTIONS__
#define CHESSFUNCTIONS__

#include <vector>
#include <functional>
#include "ChessBoard.hpp"

namespace ChessFunctions
{
	std::vector<ChessBoard> getPossibleMoves(ChessBoard cb);
	bool ownPiece(ChessPiece cp, PlayerColour turn);
	int countPieces(/*const*/ ChessBoard &cb, std::function<bool(ChessPiece)> test);
	int countPieces(/*const*/ ChessBoard &cb, ChessPiece cp);
};

#endif