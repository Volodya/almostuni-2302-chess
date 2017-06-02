/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSENGINE__
#define CHESSENGINE__

#include "ChessBoard.hpp"
#include <list>
#include <thread>

class ChessEngine;

class ChessEngineWorker
{
	friend class ChessEngine;
	
	typedef std::pair<double, ChessBoard::ptr> WeightBoardPair;
	
	bool pleaseStop; // request to stop received
	
	std::thread thread; // the thread that we run this worker in
	
	std::list<WeightBoardPair> positionPreferences;
	
	ChessEngineWorker();

	void stop();
	void startNextMoveCalculation(ChessBoard::ptr original); // this is what performs execution
};

class ChessEngine
{
	ChessBoard::ptr curPos;
	ChessEngineWorker worker;
	
public:
	void setCurPos(ChessBoard::ptr newPos);
	ChessBoard::ptr getCurPos() const;
	
	// TODO: implement calculation of the next move
	void startNextMoveCalculation();
	ChessBoard::ptr getNextMove();
	
	friend ChessEngineWorker;
};

#endif