/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef CHESSBOARDHASH__
#define CHESSBOARDHASH__

#include <array>

typedef std::array<unsigned long long, 2> ChessBoardHash;

bool operator<(const ChessBoardHash& l, const ChessBoardHash& r);
void operator^=(ChessBoardHash& l, const ChessBoardHash& r);

ChessBoardHash* generateRandomChessBoardHash();

#endif