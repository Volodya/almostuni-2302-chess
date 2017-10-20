/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessEngine.hpp"
#include <iostream>
#include "Log.hpp"
#include <algorithm>
#include <cmath>
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
	assert(original!=nullptr);
	pleaseStop=false;
	thread = std::thread( &ChessEngineWorker::startNextMoveCalculationInternal, this, original, startDepth);
}

ChessBoardAnalysis::ptr ChessEngineWorker::calculation(ChessBoardAnalysis::ptr analysis, int depth,
		weight_type alpha, weight_type beta, ChessPlayerColour maximizingPlayer)
{
	if(this->pleaseStop)
	{
		throw ChessEngineWorkerInterruptedException();
	}
	
	analysis->calculatePossibleMoves(); // must be first, even before depth check
	if(depth==0)
	{
		return analysis;
	}
	if(analysis->isCheckMate() /* || node.isDraw() */)
	{
		Log::info("i found a checkmate!");
		return analysis;
	}
	auto answers = analysis->getPossibleMoves();
	if(answers->empty())
	{
		return analysis;
	}
	ChessBoardAnalysis::ptr res=nullptr;
	
	weight_type v;
	std::function<bool(weight_type, weight_type)> testBetterV;
	std::function<weight_type(weight_type, weight_type)> newAlpha, newBeta;
	if(maximizingPlayer == analysis->getBoard()->getTurn())
	{
		v = ChessBoardAnalysis::MIN_WEIGHT;
		testBetterV = std::less<weight_type>();
		newAlpha = [](weight_type alpha, weight_type v) {
			return std::max(alpha, v);
		};
		newBeta = [](weight_type beta, weight_type v) {
			return beta;
		};
	}
	else
	{
		v = ChessBoardAnalysis::MAX_WEIGHT;
		testBetterV = std::greater<weight_type>();
		newAlpha = [](weight_type alpha, weight_type v) {
			return alpha;
		};
		newBeta = [](weight_type beta, weight_type v) {
			return std::min(beta, v);
		};
	}

	for(auto it=answers->begin(), answersEnd=answers->end(); it!=answersEnd; ++it)
	{
		// make new analysis
		ChessBoardAnalysis::ptr analysis(new ChessBoardAnalysis(*it));

		// we are changing res only if v also changes
		auto potentialRes = calculation(std::move(analysis), depth-1, alpha, beta, maximizingPlayer);
		auto potentialV = potentialRes->chessPositionWeight()*getWeightMultiplier(maximizingPlayer);
		
		if(potentialV==-99999999999ll)
		{
			Log::info("found 99999999999");
		}
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
	return res;
};

void ChessEngineWorker::startNextMoveCalculationInternal(ChessBoard::ptr original, int startDepth)
{
	assert(original!=nullptr);
	
	try
	{
		int depth = startDepth;
		auto originalAnalysis = ChessBoardAnalysis::ptr(new ChessBoardAnalysis(original));
		
		do
		{
			try
			{
				ChessBoardAnalysis::ptr best = calculation(originalAnalysis, depth,
					ChessBoardAnalysis::MIN_WEIGHT, ChessBoardAnalysis::MAX_WEIGHT,
					original->getTurn());

				Log::info("found best move");
				Log::info(ChessMove::generateCompleteMoveChain(best->getBoard()));
				Log::info(std::to_string(best->chessPositionWeight()));

				positionPreferences.emplace_front(best->chessPositionWeight(), best->getBoard());
				++depth;
			}
			catch(std::bad_alloc& e)
			{
				Log::info(std::string("i ran out of memory. depth was ")+std::to_string(depth));
				pleaseStop=true;
			}
			catch(ChessEngineWorkerInterruptedException& e)
			{
			}
		} while(!pleaseStop);
	}
	catch(std::exception &e)
	{
		std::cerr << "Worker Thread: An exception has been thrown: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Worker Thread: Some unknown exception has been thrown." << std::endl;
	}
}

void ChessEngine::setCurPos(ChessBoard::ptr newPos)
{
	curPos = newPos;
}

void ChessEngine::makeMove(ChessBoard::ptr move)
{
	assert(move!=nullptr);

	worker.stop();
	
	curPos->clearPossibleMoves(move);
	curPos = move;
	
	startNextMoveCalculation();
}

void ChessEngine::startNextMoveCalculation()
{
	worker.startNextMoveCalculation(curPos, START_DEPTH);
}

ChessBoard::ptr ChessEngine::getNextBestMove()
{
	// read the best position from worker.positionPreferences
	if(worker.positionPreferences.empty())
	{
		return nullptr;
	}
	
	Log::info(std::string("weight: ") + std::to_string(worker.positionPreferences.begin()->first/1000000));
	ChessBoard::ptr result = worker.positionPreferences.begin()->second;
	if(result==curPos) // nothing has been found since the previous time
	{
		Log::info("No new result has been found");
		return nullptr;
	}
	auto next = result;
	for(;;)
	{
		auto move = next->getMove();
		//Log::info(move->getNotation());
		auto previous = move->getFrom();
		assert(previous!=nullptr);
		if(previous==curPos)
		{
			break;
		}
		next = previous;
	}
	return next;
}
