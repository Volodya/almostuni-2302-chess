/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef RANDOMGENERATOR__
#define RANDOMGENERATOR__

#include <random>

class RandomGenerator
{
private:
	static RandomGenerator* instance;
	RandomGenerator();
	
	std::random_device rd;
public:
	static RandomGenerator* getInstance();
    std::mt19937 gen;
};

#endif