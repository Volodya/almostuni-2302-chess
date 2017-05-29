/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessEngine.hpp"

ChessEngineWorker::ChessEngineWorker()
	: pleaseStop(false)
{}

void ChessEngineWorker::stop()
{
	pleaseStop=true;
}

void ChessEngineWorker::operator()(ChessBoard::ptr original)
{
	auto weightCompare = [](
		WeightBoardPair &a, WeightBoardPair &b)
		{
			return a.first > b.first;
		};
	
	// adding the original position as the "best" so far
	calculatePossibleMoves(original);
	positionPreferences.emplace_front(original->weight(), original);
	
	while(!pleaseStop)
	{
		// select the best possible position
		auto best=positionPreferences.begin();
		
		// from the best position find possible moves
		//calculatePossibleMoves(best->second);
		auto newElements = best->second->getPossibleMoves();

		// remove the original position from the list
		positionPreferences.erase(best);
		
		// add these possible moves to the list keeping the list sorted
		std::list<WeightBoardPair> newElementsList;
		for(auto it=newElements.begin(); it!=newElements.end(); ++it)
		{
			calculatePossibleMoves(*it);
			newElementsList.emplace_back((*it)->weight(), *it);
		}
		newElementsList.sort(weightCompare);
		positionPreferences.merge(newElementsList, weightCompare);		
	}
}

void ChessEngine::startNextMoveCalculation()
{
	// start woker::thread
}

ChessBoard::ptr getNextMove()
{
	// read the best position from worker.positionPreferences
}