/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "RandomGenerator.hpp"

RandomGenerator* RandomGenerator::instance=nullptr;

RandomGenerator::RandomGenerator()
	: rd(), gen(rd())
{}

RandomGenerator* RandomGenerator::getInstance()
{
	if(!instance)
	{
		instance = new RandomGenerator;
	}
	return instance;
}