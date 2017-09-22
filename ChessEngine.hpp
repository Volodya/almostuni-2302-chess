/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSENGINE__
#define CHESSENGINE__

#include "config.hpp"

#include "ChessBoard.hpp"
#include "ChessBoardAnalysis.hpp"
#include <list>
#include <functional>
#include <thread>

class ChessEngine;

class ChessEngineWorker
{
	friend class ChessEngine;
	
	struct DepthPosition
	{
		int depth;
		ChessBoardAnalysis::ptr analysis;
		DepthPosition(int depth_, ChessBoardAnalysis::ptr analysis_)
			: depth(depth_), analysis(analysis_) {}
	};
		
	typedef std::pair<double, ChessBoard::ptr> WeightBoardPair;
	
	bool pleaseStop; // request to stop received
	ChessBoard::ptr original;
	
	std::thread thread; // the thread that we run this worker in
	
	std::list<WeightBoardPair> positionPreferences;
	
	ChessEngineWorker();

	void stop();
	void startNextMoveCalculation(ChessBoard::ptr original, int startDepth); // this is what starts the thread
	void startNextMoveCalculationInternal(ChessBoard::ptr original, int startDepth); // this is what performs execution
	
	ChessBoardAnalysis::ptr calculation(ChessBoardAnalysis::ptr analysis, int depth,
		double alpha, double beta, ChessPlayerColour maximizingPlayer);
};
class ChessEngineWorkerInterruptedException
{};

class ChessEngine
{
	ChessBoard::ptr curPos;
	ChessEngineWorker worker;
	
	int START_DEPTH = 0;
public:
	void setCurPos(ChessBoard::ptr newPos);
	ChessBoard::ptr getCurPos() const;
	
	// TODO: implement calculation of the next move
	void startNextMoveCalculation();
	ChessBoard::ptr getNextMove();
	
	friend ChessEngineWorker;
};

#endif