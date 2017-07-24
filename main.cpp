/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include <iostream>
#include "ChessBoard.hpp"
#include "ChessBoardFactory.hpp"
#include "ChessEngine.hpp"

#include "Log.hpp"

#include "ChessBoardAnalysis.hpp" // temporary

#include <memory>

int main()
{
	ChessBoardFactory factory;
	auto cb = factory.createBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//auto cb = factory.createBoard("4k3/8/8/8/8/8/8/R3K2R w KQkq - 0 1");
	
	ChessEngine ce;
	ce.setCurPos(cb);
	ce.startNextMoveCalculation();
	
	std::cin.get();
	std::cout << "Getting next move" << std::endl;
	auto best = ce.getNextMove();
	std::cout << "Next move has been received" << std::endl;
	if(best)
	{
		best->debugPrint();
		do
		{
			//best->debugPrint();
			auto move = best->getMove();
			std::cout << move->getNotation() << std::endl;
			best=move->getFrom();
		} while(best);
	}
	else
	{
		std::cout << "There's no best" << std::endl;
	}
	
	return 0;
}