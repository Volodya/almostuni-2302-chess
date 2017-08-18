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
#include <memory>
#include <new> // std::bad_alloc
#include <cassert>

ChessEngineWorker::ChessEngineWorker()
	: readyResults([](const ChessBoardHash& l, const ChessBoardHash& r){return l<r;}), pleaseStop(false)
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

/*
01 function negamax(node, depth, α, β, color)
02     if depth = 0 or node is a terminal node
03         return color * the heuristic value of node

04     childNodes := GenerateMoves(node)
05     childNodes := OrderMoves(childNodes)
06     bestValue := −∞
07     foreach child in childNodes
08         v := −negamax(child, depth − 1, −β, −α, −color)
09         bestValue := max( bestValue, v )
10         α := max( α, v )
11         if α ≥ β
12             break
13     return bestValue
*/
void ChessEngineWorker::startNextMoveCalculation(ChessBoard::ptr original, int startDepth)
{
	thread = std::thread( &ChessEngineWorker::startNextMoveCalculationInternal, this, original, startDepth);
}

void ChessEngineWorker::startNextMoveCalculationInternal(ChessBoard::ptr original, int startDepth)
{
	std::function<ChessBoardAnalysis::ptr(ChessBoardAnalysis::ptr,int,double,double,ChessPlayerColour)> calculation;
	calculation = [this, &calculation](ChessBoardAnalysis::ptr analysis, int depth,
		double alpha, double beta, ChessPlayerColour maximizingPlayer)
	{
		if(this->pleaseStop)
		{
			throw ChessEngineWorkerInterruptedException();
		}
		
		auto curHash=analysis->getBoardHash();
		if(this->readyResults.count(curHash)!=0 && this->readyResults.at(curHash).depth >= depth)
		{
			return this->readyResults.at(curHash).analysis;
		}
		
		//analysis->getBoard()->debugPrint();
		if(depth==0)
		{
			return analysis;
		}
		if(analysis->isCheckMate() /* || node.isDraw() */)
		{
			readyResults.emplace(curHash, DepthPosition(depth, analysis));
			return analysis;
		}
		auto answers = analysis->getPossibleMoves();
		if(answers.empty())
		{
			readyResults.emplace(curHash, DepthPosition(depth, analysis));
			return analysis;
		}
		ChessBoardAnalysis::ptr res=nullptr;
		
		double v;
		std::function<bool(double, double)> testBetterV;
		std::function<double(double, double)> newAlpha, newBeta;
		if(maximizingPlayer == analysis->getBoard()->getTurn())
		{
			v = -INFINITY;
			testBetterV = std::less<double>();
			newAlpha = [](double alpha, double v) {
				return std::max(alpha, v);
			};
			newBeta = [](double beta, double v) {
				return beta;
			};
		}
		else
		{
			v = INFINITY;
			testBetterV = std::greater<double>();
			newAlpha = [](double alpha, double v) {
				return alpha;
			};
			newBeta = [](double beta, double v) {
				return std::min(beta, v);
			};
		}

		for(auto it=answers.begin(); it!=answers.end(); ++it)
		{
			// check database if the analysis is already there (by hash+depth)
			
			// make new analysis
			ChessBoardAnalysis::ptr analysis(new ChessBoardAnalysis(*it));

			// we are changing res only if v also changes
			auto potentialRes = calculation(std::move(analysis), depth-1, alpha, beta, maximizingPlayer);
			auto potentialV = potentialRes->chessPositionWeight()*getWeightMultiplier(maximizingPlayer);
			
			// record the weight+depth+hash if it's new
			
			if(testBetterV(v, potentialV))
			{
				res = std::move(potentialRes);
				v = potentialV;
			}
			alpha = newAlpha(alpha, v);
			beta = newBeta(beta, v);
			if(beta <= alpha)
			{
				// remove unneeded part of the tree
				break;
			}
		}
			readyResults.emplace(curHash, DepthPosition(depth, res));
		return res;
	};
	
	int depth = startDepth;
	
	do
	{
		std::cout << "i am thinking" << std::endl;
		try
		{
			ChessBoardAnalysis::ptr best = calculation(ChessBoardAnalysis::ptr(new ChessBoardAnalysis(original)),
				depth, -INFINITY, INFINITY, original->getTurn());

			positionPreferences.emplace_front(best->chessPositionWeight(), best->getBoard());
			//std::cout << "Depth " << depth << " has been calculated" << std::endl;
			//std::cout << " current best move is" << std::endl;
			//positionPreferences.begin()->second->debugPrint();
			++depth;
		}
		catch(std::bad_alloc& e)
		{
			std::cerr << "i ran out of memory. depth was " << depth << std::endl;
			pleaseStop=true;
		}
		catch(ChessEngineWorkerInterruptedException& e)
		{
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
	std::cout << "Thread started" << std::endl;
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