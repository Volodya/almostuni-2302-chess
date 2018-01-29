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

#include "ChessBoardFactory.hpp" // temporary

ChessEngineWorker::ChessEngineWorker()
	: pleaseStop(false)
{}

ChessEngineWorker::Functions_t::Functions_t
	(ChessEngineWorker::Functions_t::TestBetterV_t testBetterV_,
	 ChessEngineWorker::Functions_t::NewAlphaBeta_t newAlpha_,
	 ChessEngineWorker::Functions_t::NewAlphaBeta_t newBeta_)
	: testBetterV(testBetterV_), newAlpha(newAlpha_), newBeta(newBeta_)
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

ChessEngineWorker::Functions_t ChessEngineWorker::functions[2] = 
	{
		ChessEngineWorker::Functions_t(
			std::greater<weight_type>(),
			[](weight_type alpha, weight_type v) { return std::max(alpha, v); },
			[](weight_type beta, weight_type v) { return beta; }
		),
		ChessEngineWorker::Functions_t(
			std::less<weight_type>(),
			[](weight_type alpha, weight_type v) { return alpha; },
			[](weight_type beta, weight_type v) { return std::min(beta, v); }
		)
	};
ChessBoardAnalysis* ChessEngineWorker::calculation(ChessBoardAnalysis* analysis, int depth,
		weight_type alpha, weight_type beta, ChessPlayerColour maximizingPlayer)
{
	//Log::info(std::string("start calculation. depth=") + std::to_string(depth) + std::string(" player=") + std::to_string((int)maximizingPlayer));
	//Log::info(std::string("board turn=") + std::to_string((int)analysis->getBoard()->getTurn()));
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
		return analysis;
	}
	auto possibleMoves = analysis->getPossibleMoves();
	if(possibleMoves->empty())
	{
		return analysis;
	}
	ChessBoardAnalysis* res=nullptr;
	
	weight_type v;
	size_t functionsNum;
	
	if(maximizingPlayer == analysis->getBoard()->getTurn())
	{
		//Log::info("first set");
		v = ChessBoardAnalysis::MIN_WEIGHT;
		functionsNum = 0;
	}
	else
	{
		//Log::info("second set");
		v = ChessBoardAnalysis::MAX_WEIGHT;
		functionsNum = 1;
	}

	for(size_t i=0, end=possibleMoves->size(); i<end; ++i)
	{
		// take up memory
		possibleMoves->at(i)->makeIFrame();
		
		// make new analysis
		ChessBoardAnalysis* analysis = ChessBoard::getAnalysis(possibleMoves->at(i));

		// we are changing res only if v also changes
		auto potentialRes = calculation(std::move(analysis), depth-1, alpha, beta, maximizingPlayer);
		auto potentialV = potentialRes->chessPositionWeight()*getWeightMultiplier(maximizingPlayer);
		
		//Log::info(std::string("test ")+std::to_string(potentialV)+std::string(" ")+std::to_string(v)+std::string(" ")+std::to_string(testBetterV(potentialV, v)));
		if(functions[functionsNum].testBetterV(potentialV, v))
		{			
			//Log::info("test for better v passed");
			res = std::move(potentialRes);
			v = potentialV;
			
			if(i) // if(i>0)
			{
				// releasing memory of old best
				possibleMoves->at(0)->makePFrame();
				
				std::swap<ChessBoard::ptr>(possibleMoves->operator[](0), possibleMoves->operator[](i));
			}
		}
		else
		{
			// release memory
			possibleMoves->at(i)->makePFrame();
		}
		alpha = functions[functionsNum].newAlpha(alpha, v);
		beta = functions[functionsNum].newBeta(beta, v);
		if(beta <= alpha)
		{
			// remove unneeded part of the tree
			//possibleMoves->at(i)->clearPossibleMoves();
			break;
		}
	}
	//Log::info("end calculation. depth=" + std::to_string(depth) + std::string(" player=") + std::to_string((int)maximizingPlayer));
	assert(res!=nullptr);
	return res;
};

void ChessEngineWorker::startNextMoveCalculationInternal(ChessBoard::ptr original, int startDepth)
{
	assert(original!=nullptr);
	
	try
	{
		int depth = startDepth;
		auto originalAnalysis = ChessBoard::getAnalysis(original);
		
		do
		{
			try
			{
				ChessBoardAnalysis* best = calculation(originalAnalysis, depth,
					ChessBoardAnalysis::MIN_WEIGHT, ChessBoardAnalysis::MAX_WEIGHT,
					original->getTurn());

				Log::info(std::string("found best move. depth=")+std::to_string(depth));
				Log::info(ChessMove::generateCompleteMoveChain(best->getBoard()));
				Log::info(std::to_string(best->chessPositionWeight()/(double)(PIECE_WEIGHT_MULTIPLIER*PIECE_PRESENT_MILTIPLIER)));

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

	worker.positionPreferences.resize(0);
	
	Log::info(std::string("Before clearPossibleMoves ")+std::to_string(ChessBoard::chessBoardCount));
	
	curPos->clearPossibleMoves(move);
	
	Log::info(std::string("After clearPossibleMoves ")+std::to_string(ChessBoard::chessBoardCount));
/*	
	int i=0;
	for(auto it=ChessBoardFactory::allBoards.begin(); it!=ChessBoardFactory::allBoards.end() && i<10; ++it)
	{
		if(auto cb=it->lock())
		{
			cb->debugPrint();	
			++i;
		}
	}
*/
	curPos = move;
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
	
	ChessBoard::ptr result = worker.positionPreferences.begin()->second;
	if(result==curPos) // nothing has been found since the previous time
	{
		Log::info("No new result has been found");
		return nullptr;
	}
	auto next = result;
	for(;;)
	{
		auto previous = next->getFrom();
		assert(previous!=nullptr);
		if(previous==curPos)
		{
			break;
		}
		next = previous;
	}
	return next;
}

void ChessEngine::stop()
{
	worker.stop();
}