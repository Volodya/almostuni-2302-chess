/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessMove.hpp"

#include <cassert>
#include "Log.hpp"

ChessMove::ChessMove()
	: previous(false), to(nullptr), from(nullptr)
{}

bool ChessMove::isMovePossible() const
{
	assert(to!=nullptr);

	ChessPiece kingPiece;
	auto width = to->getWidth();
	auto height = to->getHeight();
	if(to->getTurn()==ChessPlayerColour::WHITE)
	{
		// if it's white to move, we are looking for a black king
		kingPiece = KING_BLACK;
	}
	else
	{
		kingPiece = KING_WHITE;
	}
	size_t king[2]={}; // TODO: change this monstrocity!!!!
	bool found=false;
	for(size_t pos=0, end=to->getCellCount(); pos<end; ++pos)
	{
		if(to->getPiecePos(pos)==kingPiece)
		{
			found = true;
			king[0] = pos / width;
			king[1] = pos % width;
			break;
		}
	}
	auto possiblePieces = ChessBoard::possiblePieces;
	for(
		auto attackingPiece = possiblePieces->begin(), end=possiblePieces->end();
		attackingPiece != end;
		++attackingPiece )
	{
		if(*attackingPiece == EMPTY_CELL || getColour(*attackingPiece) != turn)
		{
			continue;
		}
		const MoveTemplate* takeMove = moveParameters.at(*attackingPiece)->takeMove;
		for(auto dir=takeMove->begin(); dir!=takeMove->end(); ++dir)
		{
			for(auto pos=dir->begin(); pos!=dir->end(); ++pos)
			{
				size_t rank = (int)king[0] - (int)pos->first;
				size_t file = (int)king[1] - (int)pos->second;
				if(rank >= height || file >= width ) // TODO: change to board dimentions
				{
					break;
				}
				auto piece = to->getPiecePos(file, rank);
				
				if(piece==*attackingPiece)
				{
					// white pawns are ganging up on white king!!!
					Log::info(std::to_string((int)*attackingPiece));
					Log::info("returning false");
					return false;
				}

				if(piece!=EMPTY_CELL)
				{
					break;
				}
			}
		}
	}
	
	Log::info("returning true");
	return true;
}

bool ChessMove::hasPrevious() const
{
	return previous;
}

void ChessMove::setFrom(ChessBoard::ptr from_)
{
	from = from_;
}
void ChessMove::setTo(ChessBoard::ptr to_)
{
	to = to_;
	turn = !to_->getTurn();
}

ChessBoard::ptr ChessMove::getFrom() const
{
	return from;
}
ChessBoard::ptr ChessMove::getTo() const
{
	return to;
}
ChessPlayerColour ChessMove::getTurn() const
{
	return turn;
}
std::string ChessMove::getNotation() const
{
	std::string result ="";
	
	if(from)
	{
		ChessPiece piece;
		int rankFrom, rankTo;
		char fileFrom, fileTo;
		for(auto it=to->begin(), end=to->end(); it!=end; ++it)
		{
			auto pos = it.getPos();
			if(*it == from->getPiecePos(pos))
			{
				continue;
			}
			
			if(*it==EMPTY_CELL)
			{
				fileFrom = it.getFile();
				rankFrom = it.getRank();
				piece = from->getPiecePos(pos);
			}
			else
			{
				fileTo = it.getFile();
				rankTo = it.getRank();
			}
		}
		
		result = chessPieceStrings[piece] + " " +
			(char)fileFrom + std::to_string(rankFrom) + " " +
			(char)fileTo + std::to_string(rankTo);
	}
	return result;
}

void ChessMove::moveAttempts(
	const ChessMoveRecordingFunction &recFunTake,
	const ChessMoveRecordingFunction &recFunDefend,
	const ChessBoard &cb, const size_t pos,
	const MoveTemplate& mt,
	bool canTake, bool canMoveToEmpty)
{
	const size_t width = cb.getWidth();
	
	const size_t rank = pos / width;
	const size_t file = pos % width;
	
	size_t newPos;
	
	for(auto direction = mt.begin(), directionEnd=mt.end(); direction != directionEnd; ++direction)
	{
		for(auto attempt = direction->begin(), attemptEnd=direction->end(); attempt != attemptEnd; ++attempt)
		{
			const int& rankShift = attempt->second;
			const int& fileShift = attempt->first;
			
			size_t newFile = (int)file + fileShift;
			size_t newRank = (int)rank + rankShift;
			
			if( newFile >= width )
			{
				break;
			}
			
			newPos = cb.getPos(newFile, newRank);
			
			if( newPos >= cb.getCellCount() )
			{
				break;
			}
			
			if(!cb.isEmptyPos(newPos))
			{
				if(canTake)
				{
					
					if(getColour(cb.getPiecePos(newPos)) != cb.getTurn())
					{
						recFunTake(pos, newPos);
					}
					recFunDefend(pos, newPos);
				}
				break; // stop if a cell isn't empty
			}
			else // if empty
			{
				if(canMoveToEmpty)
				{
					recFunTake(pos, newPos);
				}
				recFunDefend(pos, newPos);
			}
		}
	}
}
