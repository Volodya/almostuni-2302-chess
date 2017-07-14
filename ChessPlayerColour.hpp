/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSPLAYERCOLOUR__
#define CHESSPLAYERCOLOUR__

#include "config.hpp"

#include <cstddef>

enum class ChessPlayerColour
{
	WHITE, BLACK
};

constexpr ChessPlayerColour operator!(ChessPlayerColour c)
{
	return (c==ChessPlayerColour::WHITE) ? ChessPlayerColour::BLACK : ChessPlayerColour::WHITE;
}

constexpr double getWeightMultiplier(ChessPlayerColour c)
{
	return (c==ChessPlayerColour::WHITE) ? 1.0 : -1.0;
}

constexpr size_t toArrayPosition(ChessPlayerColour c)
{
	return (c==ChessPlayerColour::WHITE) ? (size_t)0 : (size_t)1;
}

#endif