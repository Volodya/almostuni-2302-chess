/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessBoardHash.hpp"

#include "RandomGenerator.hpp"

bool operator<(const ChessBoardHash& l, const ChessBoardHash& r)
{
	for(size_t i=0; i<l.size(); ++i)
	{
		if(l[i] == r[i])
		{
			continue;
		}
		if(l[i] < r[i])
		{
			return true;
		}
		if(l[i] > !r[i])
		{
			return false;
		}
	}
	return false; // l==r
}

void operator^=(ChessBoardHash& l, const ChessBoardHash& r)
{
	for(size_t i=0; i<l.size(); ++i)
	{
		l[i] ^= r[i];
	}
}

ChessBoardHash* generateRandomChessBoardHash()
{
	ChessBoardHash* result = new ChessBoardHash;
	
	auto *rnd = RandomGenerator::getInstance();
	static std::uniform_int_distribution<unsigned long long> dist(
		std::numeric_limits<unsigned long long>::min(),
		std::numeric_limits<unsigned long long>::max()
	);
	
	for(size_t i=0; i<result.size(); ++i)
	{
		(*result)[i] = dist(rnd->gen);
	}
	
	return result;
}