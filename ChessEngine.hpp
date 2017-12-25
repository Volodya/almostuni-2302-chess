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
	struct Functions_t
	{
		typedef ChessBoardAnalysis::weight_type weight_type;

		typedef std::function<bool(weight_type, weight_type)> TestBetterV_t;
		typedef std::function<weight_type(weight_type, weight_type)> NewAlphaBeta_t;

		TestBetterV_t testBetterV;
		NewAlphaBeta_t newAlpha, newBeta;
		
		Functions_t(TestBetterV_t testBetterV_, NewAlphaBeta_t newAlpha_, NewAlphaBeta_t newBeta_);
	};
	static Functions_t functions[2];

	friend class ChessEngine;
	
	typedef ChessBoardAnalysis::weight_type weight_type;
	typedef std::pair<weight_type, ChessBoard::ptr> WeightBoardPair;
	
	bool pleaseStop; // request to stop received
	ChessBoard::ptr original;
	
	std::thread thread; // the thread that we run this worker in
	
	std::list<WeightBoardPair> positionPreferences;
	
	ChessEngineWorker();

	void stop();
	void startNextMoveCalculation(ChessBoard::ptr original, int startDepth); // this is what starts the thread
	void startNextMoveCalculationInternal(ChessBoard::ptr original, int startDepth); // this is what performs execution
	
	ChessBoardAnalysis* calculation(ChessBoardAnalysis* analysis, int depth,
		weight_type alpha, weight_type beta, ChessPlayerColour maximizingPlayer);
};
class ChessEngineWorkerInterruptedException
{};

class ChessEngine
{
	ChessBoard::ptr curPos;
	ChessEngineWorker worker;
	
	int START_DEPTH = 4;
public:
	void setCurPos(ChessBoard::ptr newPos);
	void makeMove(ChessBoard::ptr move);
	ChessBoard::ptr getCurPos() const;
	
	// TODO: implement calculation of the next move
	void startNextMoveCalculation();
	ChessBoard::ptr getNextBestMove();
	
	void stop();
	
	friend ChessEngineWorker;
};

#endif