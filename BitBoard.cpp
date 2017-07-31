/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "BitBoard.hpp"

BitBoard::BitBoard(uint8_t height, uint8_t width)
	: h(height), w(width), data(((size_t)height*width+(BITBLOCKSIZE-1))/BITBLOCKSIZE)
{}

uint8_t BitBoard::getHeight() const
{
	return h;
}
uint8_t BitBoard::getWidth() const
{
	return w;
}

void BitBoard::set(uint8_t f, uint8_t r, bool val)
{
	size_t bit = f+r*w;
	size_t dword = bit / BITBLOCKSIZE;
	bit %= BITBLOCKSIZE;
	this->data[dword].set(bit, val);
}
bool BitBoard::get(uint8_t f, uint8_t r) const
{
	size_t bit = f+r*w;
	size_t dword = bit / BITBLOCKSIZE;
	bit %= BITBLOCKSIZE;
	return this->data[dword].test(bit);
}

int BitBoard::countBits() const
{
	int res = 0;
	for(auto it=data.begin(); it!=data.end(); ++it)
	{
		res += it->count();
	}
	return res;
}

std::pair<uint8_t, uint8_t> BitBoard::getFirstOccurance() const
{
	for(size_t dword=0; dword<data.size(); ++dword)
	{
		if(data[dword].none()) continue;
		
		for(size_t bit=0; bit<BITBLOCKSIZE; ++bit)
		{
			if(data[dword].test(bit))
			{
				int setbit = dword*BITBLOCKSIZE + bit;
				return {setbit % w, setbit / w};
			}
		}
	}
	return {0, 0};
}