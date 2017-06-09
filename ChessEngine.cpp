/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessEngine.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <functional>

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
/*
01 function alphabeta(node, depth, α, β, maximizingPlayer)
02      if depth = 0 or node is a terminal node
03          return the heuristic value of node
04      if maximizingPlayer
05          v := -∞
06          for each child of node
07              v := max(v, alphabeta(child, depth – 1, α, β, FALSE))
08              α := max(α, v)
09              if β ≤ α
10                  break (* β cut-off *)
11          return v
12      else
13          v := +∞
14          for each child of node
15              v := min(v, alphabeta(child, depth – 1, α, β, TRUE))
16              β := min(β, v)
17              if β ≤ α
18                  break (* α cut-off *)
19          return v
*/

void ChessEngineWorker::startNextMoveCalculation(ChessBoard::ptr original, int startDepth)
{
	std::function<ChessBoard::ptr(ChessBoard::ptr,int,double,double,PlayerColour)> calculation;
	calculation = [this, &calculation](ChessBoard::ptr node, int depth,
		double alpha, double beta, PlayerColour maximizingPlayer)
	{
		calculatePossibleMoves(node);
		if(depth==0 || node->isCheckMate() /* || node.isDraw() */)
		{
			return node;
		}
		
		auto answers = node->getPossibleMoves();
		ChessBoard::ptr res;
		if(maximizingPlayer == node->getTurn())
		{
			double v = -INFINITY;
			for(auto it=answers.begin(); it!=answers.end(); ++it)
			{
				res = calculation(*it, depth-1, alpha, beta, maximizingPlayer);
				v = std::max(v, res->weight()*maximizingPlayer);
				alpha = std::max(alpha, v);
				if(beta <= alpha)
				{
					break;
				}
			}
		}
		else
		{
			double v = INFINITY;
			for(auto it=answers.begin(); it!=answers.end(); ++it)
			{
				res = calculation(*it, depth-1, alpha, beta, maximizingPlayer);
				v = std::min(v, res->weight()*maximizingPlayer);
				beta = std::min(v, beta);
				if(beta <= alpha)
				{
					break;
				}
			}
		}
		return res;
	};
	
	int depth = startDepth;
	do
	{
		std::cout << "i am thinking" << std::endl;
		auto best = calculation(original, depth, -INFINITY, INFINITY, original->getTurn());
		positionPreferences.emplace_front(best->weight(), best);
		++depth;
	} while(!pleaseStop);
}

/*
void ChessEngineWorker::startNextMoveCalculation(ChessBoard::ptr original)
{
	auto calculation = [original, this]()
	{
		auto weightCompareUs = [](
			WeightBoardPair a, WeightBoardPair b)
			{
				return a.first > b.first;
			};
		auto weightCompareOpponent = [](
			ChessBoard::ptr a, ChessBoard::ptr b)
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
			for(size_t i=0; i<newElements.size(); ++i)
			{
				std::cout << " ELEMENT " << i << " " << newElements[i] << std::endl;
				std::cout << "  NEXT TURN " << newElements[i] << std::endl;
				newElements[i]->debugPrint();
				
				calculatePossibleMoves(newElements[i]);
				auto responses = newElements[i]->getPossibleMoves();
								
				std::cout << "   NUMBER OF POSSIBLE MOVES: " << newElements[i]->getPossibleMoves().size() << std::endl;
				
				std::cout << "   RESPONSES: ";
				for(auto it2=responses.begin(); it2!=responses.end(); ++it2)
				{
					std::cout << '[' << (*it2) << ' ' << (*it2)->getTurn() << ((*it2)->possibleMovesCalculated?"c":"") << "] ";
				}
				std::cout  << std::endl;
				
				auto bestResponse =
					std::max_element(responses.begin(), responses.end(), weightCompareOpponent);
				newElements[i] = *bestResponse;
				
				std::cout << "    NEXT NEXT " << newElements[i] << std::endl;
				newElements[i]->debugPrint();

			}
			//for(auto it=newElements.begin(); it!=newElements.end(); ++it)
			//{
				//std::cout << "  NEXT TURN" << std::endl;
				//(*it)->debugPrint();
				
				//calculatePossibleMoves(*it);
				//auto responses = (*it)->getPossibleMoves();
				
				//std::cout << "   NUMBER OF POSSIBLE MOVES: " << responses.size() << std::endl;
				
				//for(auto it=responses.begin(); it!=responses.end(); ++it)
				//{
					//std::cout << (*it)->getTurn();
				//}
				//std::cout  << std::endl;
				
				//auto bestResponse =
					//std::max_element(responses.begin(), responses.end(), weightCompareOpponent);
				//*it = *bestResponse;
				
				//std::cout << "    NEXT NEXT" << std::endl;
				//(*it)->debugPrint();

			//}

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
			
			// TEMPORARY
			for(auto it=positionPreferences.begin(); it!=positionPreferences.end(); ++it)
			{
				std::cout << it->second->getTurn();
				if(it->second->getTurn()==-1) pleaseStop = true;
				pleaseStop=true;
			}
			std::cout  << std::endl;
			// TEMPORARY

		} while(!pleaseStop);
	};
	thread = std::thread(calculation);
}
*/
void ChessEngine::setCurPos(ChessBoard::ptr newPos)
{
	curPos = newPos;
}

void ChessEngine::startNextMoveCalculation()
{
	worker.startNextMoveCalculation(curPos, START_DEPTH);
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