/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#ifndef BITBOARD__
#define BITBOARD__

#include <cstdint>
#include <vector>
#include <bitset>
#include <pair>

class BitBoard
{
	int8_t h, w;
	std::vector<std::bitset<32>> data;
public:
	BitBoard() = delete;
	BitBoard(int8_t height, int8_t width);
	int8_t getHeight() const;
	int8_t getWidth() const;
	void set(int8_t f, int8_t r, bool val);
	bool get(int8_t f, int8_t r) const;
	int countBits() const;
	std::pair<int8_t, int8_t> getFirstOccurance() const; // file, rank
	BitBoard operator&(const BitBoard& that) const;
	BitBoard operator&=(const BitBoard& that);
	BitBoard operator|(const BitBoard| that) const;
	BitBoard operator|=(const BitBoard& that);
};

#endif