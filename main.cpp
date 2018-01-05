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
	try
	{
		ChessBoardFactory factory;
		auto cb = factory.createBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
		//auto cb = factory.createBoard("4k3/8/8/8/8/8/8/qR4K1 w KQkq - 0 1");
		cb->debugPrint();
		
		ChessBoardAnalysis analysis(cb);
		analysis.calculatePossibleMoves();
		//std::cout << std::boolalpha << "isCheck " << analysis.isCheck() << std::endl;
		//std::cout << std::boolalpha << "isCheckMate " << analysis.isCheckMate() << std::endl;
		//std::cout << "Chess Pieces Weight " << analysis.chessPiecesWeight() << std::endl;
		//std::cout << "Chess Pieces Attacked Weight " << analysis.chessPieceAttackedWeight() << std::endl;
		//std::cout << "Chess Control Weight " << analysis.chessCentreControlWeight() << std::endl;
		//std::cout << "Chess Position Weight " << analysis.chessPositionWeight() << std::endl;
		/*
		auto possible = analysis.getPossibleMoves();
		for(auto it=possible->begin(); it!=possible->end(); ++it)
		{
			ChessBoardAnalysis a(*it);
			a.calculatePossibleMoves();
		}
		
		std::cout << "ChessBoardCount " << ChessBoard::chessBoardCount << std::endl;
		std::cout << "ChessMoveCount " << ChessMove::chessMoveCount << std::endl;
		
		cb->clearPossibleMoves();
		
		std::cout << "ChessBoardCount " << ChessBoard::chessBoardCount << std::endl;
		std::cout << "ChessMoveCount " << ChessMove::chessMoveCount << std::endl;
		{
			ChessEngine engine;
			engine.setCurPos(cb);
			engine.startNextMoveCalculation();
			std::cin.get();
			engine.stop();
			
			std::cout << "ChessBoardCount " << ChessBoard::chessBoardCount << std::endl;
			std::cout << "ChessMoveCount " << ChessMove::chessMoveCount << std::endl;

			cb->clearPossibleMoves();
			
			std::cout << "ChessBoardCount " << ChessBoard::chessBoardCount << std::endl;
			std::cout << "ChessMoveCount " << ChessMove::chessMoveCount << std::endl;
		}
		
		for(auto it=ChessBoardFactory::allBoards.begin(); it!=ChessBoardFactory::allBoards.end(); ++it)
		{
			if(auto cb=it->lock())
			{
				cb->debugPrint();
				std::cout << cb->getMove() << std::endl;
			}
		}
		*/

		//return 0;
		
		//assert(cb->hasPrevious()==false);
		//assert(cb->getFrom()==nullptr);
		
		ChessEngine engine;
		engine.setCurPos(cb);
		for(;;)
		{
			engine.startNextMoveCalculation();
			
			ChessBoardAnalysis::constructed=0;
			//auto start = std::chrono::high_resolution_clock::now();
			std::cin.get();
			//auto end = std::chrono::high_resolution_clock::now();
			engine.stop();
			
			//long double duration = (end-start).count();
			//duration /= 1000000000;
			
			//std::cout << ChessBoardAnalysis::constructed << '/' << duration << ' ' << ChessBoardAnalysis::constructed / duration << std::endl;
			
			std::cout << "Number of made ChessBoard-s: " << ChessBoard::chessBoardCount << std::endl;
			
			auto best = engine.getNextBestMove();
			
			best->debugPrint();
			
			engine.makeMove(best);
		}
	}
	catch(std::exception &e)
	{
		std::cerr << "An exception has been thrown: " << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Some unknown exception has been thrown." << std::endl;
	}
	return 0;
}