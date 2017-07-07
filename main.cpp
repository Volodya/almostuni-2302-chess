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

#include <memory>

int main()
{
	ChessBoardFactory factory;
	auto cb = factory.createBoard();
	
	//cb->debugPrint();
	//std::cout << "Weight: " << cb->weight() << std::endl;
	
	//auto tmp = cb->getPossibleMoves();
	
	//std::cout << "Number of possible moves: " << tmp.size() << std::endl;
	
	//for(auto it=tmp.begin(); it!=tmp.end(); ++it)
	{
		//calculatePossibleMoves(*it);
		//(*it)->debugPrint();
		//std::cout << "Weight: " << (*it)->weight() << std::endl;
	}
	
	ChessEngine ce;
	ce.setCurPos(cb);
	ce.startNextMoveCalculation();
	
	std::cin.get();
	auto best = ce.getNextMove();
	if(best)
	{
		do
		{
			//best->debugPrint();
			//std::cout << "Weight: " << best->weight() << std::endl;
			//best=best->getFrom();
		} while(best);
	}
	else
	{
		std::cout << "There's no best" << std::endl;
	}
	
	return 0;
}