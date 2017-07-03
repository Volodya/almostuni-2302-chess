/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessEngine.hpp"
#include "ChessBoardAnalysis.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <functional>
#include <memory>
#include <new> // std::bad_alloc
#include <cassert>

ChessEngineWorker::ChessEngineWorker()
	: pleaseStop(false)
{}

void ChessEngineWorker::stop()
{
	pleaseStop=true;
	thread.join();
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
	typedef std::unique_ptr<ChessBoardAnalysis> ChessBoardAnalysisPtr;
	std::function<ChessBoardAnalysisPtr(ChessBoardAnalysisPtr&&,int,double,double,ChessPlayerColour)> calculation;
	calculation = [this, &calculation](ChessBoardAnalysisPtr&& analysis, int depth,
		double alpha, double beta, ChessPlayerColour maximizingPlayer)
	{
		analysis->getBoard()->debugPrint();
		if(depth==0 || analysis->isCheckMate() /* || node.isDraw() */)
		{
			return std::move(analysis);
		}
		auto answers = analysis->getPossibleMoves();
		if(answers.empty())
		{
			return std::move(analysis);
		}
		ChessBoardAnalysisPtr res=nullptr;
		if(maximizingPlayer == analysis->getBoard()->getTurn())
		{
			double v = -INFINITY;
			for(auto it=answers.begin(); it!=answers.end(); ++it)
			{
				ChessBoardAnalysisPtr analysis(new ChessBoardAnalysis((*it)->getTo()));

				// we are changing res only if v also changes
				auto potentialRes = calculation(std::move(analysis), depth-1, alpha, beta, maximizingPlayer);
				auto potentialV = potentialRes->chessPositionWeight()*getWeightMultiplier(maximizingPlayer);
				std::cerr << "we are here now" << std::endl;
				if(v < potentialV)
				{
					res = std::move(potentialRes);
					v = potentialV;
				}
				alpha = std::max(alpha, v);
				if(beta <= alpha)
				{
					// remove unneeded part of the tree
					break;
				}
			}
		}
		else
		{
			double v = INFINITY;
			for(auto it=answers.begin(); it!=answers.end(); ++it)
			{
				ChessBoardAnalysisPtr analysis(new ChessBoardAnalysis((*it)->getTo()));
				
				// we are changing res only if v also changes
				auto potentialRes = calculation(std::move(analysis), depth-1, alpha, beta, maximizingPlayer);
				auto potentialV = potentialRes->chessPositionWeight()*getWeightMultiplier(maximizingPlayer);
				if(v > potentialV)
				{
					res = std::move(potentialRes);
					v = potentialV;
				}
				beta = std::min(v, beta);
				if(beta <= alpha)
				{
					// remove unneeded part of the tree
					break;
				}
			}
		}
		
		return std::move(res);
	};
	
	int depth = startDepth;
	depth = 1;
	do
	{
		std::cout << "i am thinking" << std::endl;
		try
		{
			ChessBoardAnalysisPtr best = calculation(ChessBoardAnalysisPtr(new ChessBoardAnalysis(original)),
				depth, -INFINITY, INFINITY, original->getTurn());
			positionPreferences.emplace_front(best->chessPositionWeight(), best->getBoard());
			++depth;
			pleaseStop=true;
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