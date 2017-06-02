/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessEngine.hpp"
#include <iostream>
#include <algorithm>

ChessEngineWorker::ChessEngineWorker()
	: pleaseStop(false)
{}

void ChessEngineWorker::stop()
{
	std::cerr << "trying to stop" << std::endl;
	pleaseStop=true;
	thread.join();
	std::cerr << "join completed" << std::endl;
}

void ChessEngineWorker::startNextMoveCalculation(ChessBoard::ptr original)
{
	auto calculation = [original, this]()
	{
		auto weightCompareUs = [](
			WeightBoardPair &a, WeightBoardPair &b)
			{
				return a.first > b.first;
			};
		auto weightCompareOpponent = [](
			ChessBoard::ptr &a, ChessBoard::ptr &b)
			{
				calculatePossibleMoves(a);
				calculatePossibleMoves(b);
				return a->weight() < b->weight();
			};
		
		// adding the original position as the "best" so far
		calculatePossibleMoves(original);
		positionPreferences.emplace_front(original->weight(), original);
		
		do
		{
			std::cout << "i am thinking" << std::endl;
			
			// select the best possible position
			auto best=positionPreferences.begin();
			std::cout << "It's a turn of " << best->second->getTurn() << std::endl;
			
			// from the best position find possible moves
			//calculatePossibleMoves(best->second);
			auto newElements = best->second->getPossibleMoves();
			
			// loop through newElements and calculate best response by opponent
			for(auto it=newElements.begin(); it!=newElements.end(); ++it)
			{
				std::cout << "It's a next turn of " << (*it)->getTurn() << std::endl;
				calculatePossibleMoves(*it);
				auto responses = (*it)->getPossibleMoves();
				for(auto it=responses.begin(); it!=responses.end(); ++it)
				{
					std::cout << (*it)->getTurn();
				}
				std::cout  << std::endl;
				auto bestResponse =
					std::max_element(responses.begin(), responses.end(), weightCompareOpponent);
				*it = *bestResponse;
				std::cout << "It's a next next turn of " << (*it)->getTurn() << std::endl;

			}

			// remove the original position from the list
			positionPreferences.erase(best);
			
			// add these possible moves to the list keeping the list sorted
			std::list<WeightBoardPair> newElementsList;
			for(auto it=newElements.begin(); it!=newElements.end(); ++it)
			{
				calculatePossibleMoves(*it);
				newElementsList.emplace_back((*it)->weight(), *it);
			}
			newElementsList.sort(weightCompareUs);
			positionPreferences.merge(newElementsList, weightCompareUs);
			
			for(auto it=positionPreferences.begin(); it!=positionPreferences.end(); ++it)
			{
				std::cout << it->second->getTurn();
				//if(it->second->getTurn()!=-1) pleaseStop = true;
			}
			std::cout  << std::endl;

		} while(!pleaseStop);
	};
	thread = std::thread(calculation);
}

void ChessEngine::setCurPos(ChessBoard::ptr newPos)
{
	curPos = newPos;
}

void ChessEngine::startNextMoveCalculation()
{
	worker.startNextMoveCalculation(curPos);
}

ChessBoard::ptr ChessEngine::getNextMove()
{
	// stop the calculation
	worker.stop();
	
	// read the best position from worker.positionPreferences
	if(worker.positionPreferences.empty())
	{
		return nullptr;
	}
	
	return worker.positionPreferences.begin()->second;
}