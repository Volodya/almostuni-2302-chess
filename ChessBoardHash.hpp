/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDHASH__
#define CHESSBOARDHASH__

#include <bitset>

typedef std::bitset<1024> ChessBoardHash;

ChessBoardHash generateRandomChessBoardHash();

#endif