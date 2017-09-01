/*
 * Volodya Mozhenkov (Almost University) 2017
 * Программа: 23 ФЕВРАЛЯ
 * http://www.almost-university.com/
 * GNU GPL v 3.0
 */

#include "BitBoard.hpp"

BitBoard::BitBoard(ChessGameParameters::ptr param_)
	:param(param_), data((param->getCellCount()+(BITBLOCKSIZE-1))/BITBLOCKSIZE)
{}

uint8_t BitBoard::getHeight() const
{
	return param->getHeight();
}
uint8_t BitBoard::getWidth() const
{
	return param->getWidth();
}

void BitBoard::set(uint8_t f, uint8_t r, bool val)
{
	size_t bit = f+r*param->getWidth();
	size_t dword = bit / BITBLOCKSIZE;
	bit %= BITBLOCKSIZE;
	uint32_t i = 1;
	i <<= bit;
	if(val)
	{
		this->data[dword] |= i;
	}
	else
	{
		this->data[dword] &= ~i;
	}
}
bool BitBoard::get(uint8_t f, uint8_t r) const
{
	size_t bit = f+r*param->getWidth();
	size_t dword = bit / BITBLOCKSIZE;
	bit %= BITBLOCKSIZE;
	uint32_t i = 1;
	i <<= bit;
	return (bool)(this->data[dword] & i);
}

int BitBoard::countBits() const
{
	int res = 0;
	for(auto it=data.begin(); it!=data.end(); ++it)
	{
		auto cur = *it;
		while(cur != 0)
		{
			++res;
			cur >>= 1;
		}
	}
	return res;
}

std::pair<uint8_t, uint8_t> BitBoard::getFirstOccurance() const
{
	auto w = param->getWidth();
	for(size_t dword=0; dword<data.size(); ++dword)
	{
		auto byte = data.at(dword);
		if(byte==0) continue;
		
		uint32_t i = 1 << (BITBLOCKSIZE-1);
		for(size_t bit=0; bit<BITBLOCKSIZE; ++bit)
		{
			if(byte & i)
			{
				int setbit = dword*BITBLOCKSIZE + bit;
				return {setbit % w, setbit / w};
			}
			i >>= 1;
		}
	}
	return {0, 0};
}