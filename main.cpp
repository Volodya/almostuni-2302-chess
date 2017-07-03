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
class Test
{int i;};
std::unique_ptr<Test> foo1()
{
	std::unique_ptr<Test> res(new Test);
	return res;
}
std::unique_ptr<Test> foo2(std::unique_ptr<Test>&& t)
{
	return std::move(t);
}

int main()
{
	std::cerr << "Trying first" << std::endl;
	auto test1=foo1();
	std::cerr << "First succeeded" << std::endl;
	std::cerr << "Trying second" << std::endl;
	auto test2=foo2(std::unique_ptr<Test>(new Test));
	std::cerr << "Second succeeded" << std::endl;
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