/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "BitBoard.hpp"

BitBoard::BitBoard(int8_t height, int8_t width)
	: h(height), w(width), data((height*width+7)/8)
{}

int8_t BitBoard::getHeight() const
{
	return h;
}
int8_t BitBoard::getWidth() const
{
	return w;
}

bool BitBoard::set(int8_t f, int8_t r, bool val)
{
	size_t bit = f+r*w;
	size_t dword = bit / 32;
	bit %= 32;
	this->data[dword].set(bit, val);
}
bool BitBoard::get(int8_t f, int8_t r)
{
	size_t bit = f+r*w;
	size_t dword = bit / 32;
	bit %= 32;
	return this->data[dword].test(bit);
}

int BitBoard::countBits()
{
	int res = 0;
	for(auto it=data.begin(); it!=data.end(); ++it)
	{
		res += it->count();
	}
	return res;
}

std::pair<int8_t, int8_t> BitBoard::getFirstOccurance() const
{
	for(size_t dword=0; dword<data.size(); ++dword)
	{
		if(data[dword].none()) continue;
		
		for(size_t bit=0; bit<32; ++bit)
		{
			if(data[dword].test(bit))
			{
				int setbit = dword*32 + bit;
				return {setbit % w, setbit / w};
			}
		}
	}
	return {0, 0}
}