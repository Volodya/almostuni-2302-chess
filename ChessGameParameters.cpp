/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "ChessGameParameters.hpp"

void ChessGameParameters::setDimentions(
	ChessGameParameters::BoardPosition_t w, ChessGameParameters::BoardPosition_t h)
{
	this->width=w;
	this->height=h;
	this->cellCount = h * w;
}