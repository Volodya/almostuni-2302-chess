/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include <iostream>
#include <cassert>
#include "ChessBoard.hpp"
#include "ChessBoardFactory.hpp"
#include "ChessEngine.hpp"

#include "Log.hpp"

#include "ChessBoardAnalysis.hpp" // temporary

#include <memory>
#include <chrono>

int main()
{
	ChessBoardFactory factory;
	auto cb = factory.createBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//auto cb = factory.createBoard("4k3/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	assert(cb->getMove()->hasPrevious()==false);
	assert(cb->getMove()->getFrom()==nullptr);
	Log::info( std::to_string( (unsigned long long)(cb.get()) ));
		
	ChessEngine engine;
	engine.setCurPos(cb);
	auto start = std::chrono::high_resolution_clock::now();
	engine.startNextMoveCalculation();
	std::cin.get();
	auto end = std::chrono::high_resolution_clock::now();
	long double duration = (end-start).count();
	duration /= 1000000000;
	
	std::cout << ChessBoardAnalysis::constructed << '/' << duration << ' ' << ChessBoardAnalysis::constructed / duration << std::endl;
	
	std::cout << "Getting next move" << std::endl;
	auto best = engine.getNextMove();
	std::cout << "Next move has been received" << std::endl;
	if(best)
	{
		best->debugPrint();
		for(;;)
		{
			auto move = best->getMove();
			if(!move->hasPrevious())
			{
				break;
			}
			std::cout << move->getNotation() << std::endl;
			best=move->getFrom();
		};
	}
	else
	{
		std::cout << "There's no best" << std::endl;
	}
	
	return 0;
}