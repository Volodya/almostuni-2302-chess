/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoard.hpp"
#include "ChessMove.hpp" // because it is not included in ChessBoard.hpp due to the circular dep

#include <iostream>
#include <cassert>

#include "moveTemplate.hpp"

unsigned long long ChessBoard::count=0;

ChessBoard::ChessBoard()
	: turn(ChessPlayerColour::WHITE), move(nullptr)
{
	for(int i=0; i<8; ++i)
	{
		for(int j=0; j<8; ++j)
		{
			board[i][j] = ' ';
		}
	}
	//std::cerr << "[CONST] " << ++count << std::endl;
}
ChessBoard::~ChessBoard()
{
	//std::cerr << "[DEST] " << --count << std::endl;
}

std::string ChessBoard::toFEN() const
{
	std::string result;
	
	int countEmpty=0;
	for(int rank=7; rank>=0; --rank)
	{
		for(int file=0; file<8; ++file)
		{
			if(board[rank][file] == EMPTY_CELL)
			{
				++countEmpty;
			}
			else
			{
				if(countEmpty>0)
				{
					result += std::to_string(countEmpty);
					countEmpty = 0;
				}
				result += board[rank][file];
			}
		}
		if(countEmpty>0)
		{
			result += std::to_string(countEmpty);
			countEmpty = 0;
		}
		
		if(rank>0)
		{
			result += '/';
		}
	}
	
	result += ' ';
	
	result += turn==ChessPlayerColour::WHITE ? 'w' : 'b';
	
	result += ' ';
	
	return result;
}

void ChessBoard::debugPrint() const
{
	if(turn==ChessPlayerColour::WHITE)
	{
		std::cout << "White's turn" << std::endl;
	}
	else
	{
		std::cout << "Black's turn" << std::endl;
	}
	std::cout << ' ';
	for(int i=0; i<8; ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	for(int rank=8; rank>0; --rank)
	{
		std::cout << rank;
		for(int file=0; file<8; ++file)
		{
			std::cout << ' ' << board[rank-1][file];
		}
		std::cout << std::endl;
	}
}

ChessPlayerColour ChessBoard::getTurn() const
{
	return turn;
}

void ChessBoard::placePiece(char file, int rank, ChessPiece piece)
{
	this->placePiecePos(file-'A', rank-1, piece);
}
void ChessBoard::placePiecePos(size_t file, size_t rank, ChessPiece piece)
{
	board[rank][file] = piece;
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(size_t file, size_t rank) const
{
	return board[rank][file];
}

ChessMove::ptr ChessBoard::getMove() const
{
	return move;
}
bool ChessBoard::isEmpty(char file, int rank) const
{
	return (board[rank-1][file-'A'] == ' ');
}


