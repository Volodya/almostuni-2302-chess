/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardHash.hpp"

#include "RandomGenerator.hpp"

ChessBoardHash generateRandomChessBoardHash()
{
	ChessBoardHash result;
	
	auto *rnd = RandomGenerator::getInstance();
	static std::bernoulli_distribution dist(0.5);
	
	for(size_t i=0; i<result.size(); ++i)
	{
		result[i] = dist(rnd->gen);
	}
	
	return result;
}