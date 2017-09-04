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

#include "Log.hpp"

// helper
std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> generateEmptyBitBoards(ChessGameParameters::ptr param)
{
	std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> result;
	auto possiblePieces = param->getPossiblePieces();
	for(auto it=possiblePieces.begin(); it!=possiblePieces.end(); ++it)
	{
		result[*it] = std::unique_ptr<BitBoard>(new BitBoard(param));
	}
	return result;
}
std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> copy(
	const std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> &original)
{
	std::array<std::unique_ptr<BitBoard>, KNOWN_CHESS_PIECE_COUNT> result;
	for(size_t i=0; i<KNOWN_CHESS_PIECE_COUNT; ++i)
	{
		if(original[i])
		{
			result[i] = std::unique_ptr<BitBoard>(new BitBoard(*original[i]));
		}
	}
	return result;
}

// static data members

std::vector<ChessBoardHash *> ChessBoard::pieceHashes;

// class functions

void ChessBoard::initialisePieceHashes()
{
	pieceHashes.resize(KNOWN_CHESS_PIECE_COUNT * param->getCellCount());
	pieceHashes[EMPTY_CELL] = nullptr;
	
	auto possiblePieces = param->getPossiblePieces();
	for(auto it=possiblePieces.begin(); it!=possiblePieces.end(); ++it)
	{
		for(size_t i=0, n=param->getCellCount(); i<n; ++i)
		{
			auto curHash = generateRandomChessBoardHash();
			pieceHashes[toPieceHashArrayPos(*it, i)] = curHash;
		}
	}
	pieceHashes.shrink_to_fit();
}

ChessBoard::ChessBoard(ChessGameParameters::ptr param_)
	: param(param_), board(new ChessPiece[param->getCellCount()]),
	  //bitBoards(generateEmptyBitBoards(param)),
	  turn(ChessPlayerColour::WHITE), move(nullptr),
	  hash(generateRandomChessBoardHash())
{
	Log::info("about to initialise hashes");
	initialisePieceHashes(); // technically does not belong here
	Log::info("hashes have been initialised");
	auto s = param->getCellCount();
	std::fill(board, board+s, EMPTY_CELL);
	
	// generate hashes
}
ChessBoard::ChessBoard(const ChessBoard& that)
	: param(that.param), board(new ChessPiece[param->getCellCount()]),
	  //bitBoards(copy(that.bitBoards)),
	  turn(that.turn), move(that.move),
	  hash(new ChessBoardHash(*that.hash))
{
	auto s = param->getCellCount();
	std::copy(that.board, that.board+s, this->board);
}

ChessBoard::~ChessBoard()
{
	delete[] board;
}

size_t ChessBoard::getPos(size_t file, size_t rank) const
{
	return rank*param->getWidth()+file;
}
size_t ChessBoard::toPieceHashArrayPos(const ChessPiece &piece, const size_t &position)
{
	return param->getCellCount() * ((size_t)piece - (size_t)1) + position;
}

uint8_t ChessBoard::getHeight() const
{
	return param->getHeight();
}
uint8_t ChessBoard::getWidth() const
{
	return param->getWidth();
}


std::string ChessBoard::toFEN() const
{
	std::string result;
	
	int countEmpty=0;
	ChessPiece* c=board;
	for(int rank=7; rank>=0; --rank)
	{
		for(int file=0; file<8; ++file)
		{
			if(*c == EMPTY_CELL)
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
				result += *c;
			}
			
			++c;
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
	for(uint8_t i=0; i<param->getWidth(); ++i)
	{
		std::cout << ' ' << (char)('A'+i);
	}
	std::cout << std::endl;
	{
		ChessPiece c;
		for(uint8_t rank=param->getHeight(); rank>0; --rank)
		{
			std::cout << (int)rank;
			for(uint8_t file=0; file<param->getWidth(); ++file)
			{
				c = board[getPos(file, rank-1)];
				std::cout << ' ' << chessPieceStrings[c];
			}
			std::cout << std::endl;
		}
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
	auto pos = getPos(file, rank);
	
	// update board
	ChessPiece takenPiece = board[pos];
	board[pos] = piece;
	
	// update BitBoards
	if(takenPiece != EMPTY_CELL)
	{
		//assert(bitBoards[takenPiece]!=nullptr);
		//bitBoards[takenPiece]->set(file, rank, false);
	}
	if(piece != EMPTY_CELL)
	{
		//assert(bitBoards[piece]!=nullptr);
		//bitBoards[piece]->set(file, rank, true);
	}
	
	// update hash
	if(takenPiece != EMPTY_CELL)
	{
		assert(pieceHashes.at(toPieceHashArrayPos(takenPiece, pos))!=nullptr);
		(*hash)^=*pieceHashes.at(toPieceHashArrayPos(takenPiece, pos));
	}
	if(piece != EMPTY_CELL)
	{
		assert(pieceHashes.at(toPieceHashArrayPos(piece, pos))!=nullptr);
		(*hash)^=*pieceHashes.at(toPieceHashArrayPos(piece, pos));
	}
}
ChessPiece ChessBoard::getPiece(char file, int rank) const
{
	return this->getPiecePos(file-'A', rank-1);
}
ChessPiece ChessBoard::getPiecePos(size_t file, size_t rank) const
{
	return board[getPos(file, rank)];
}

ChessMove::ptr ChessBoard::getMove() const
{
	return move;
}

ChessBoardHash ChessBoard::getHash() const
{
	return *hash;
}

bool ChessBoard::isEmpty(char file, int rank) const
{
	return this->isEmptyPos(file-'A', rank-1);
}
bool ChessBoard::isEmptyPos(size_t file, size_t rank) const
{
	return board[getPos(file, rank)] == EMPTY_CELL;
}
