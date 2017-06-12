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
#include <new> // std::bad_alloc

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
					// remove unneeded part of the tree
					(*it)->dropPossibleMoves();
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
					// remove unneeded part of the tree
					(*it)->dropPossibleMoves();
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
		try
		{
			auto best = calculation(original, depth, -INFINITY, INFINITY, original->getTurn());
			positionPreferences.emplace_front(best->weight(), best);
			++depth;
		}
		catch(std::bad_alloc& e)
		{
			std::cout << "i ran out of memory. depth was " << depth << std::endl;
			pleaseStop=true;
		}
	} while(!pleaseStop);
}

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