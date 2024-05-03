#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <iostream>
#include <cstring>
#include <list>
#include <deque>
#include <cstdlib>
#include "Memory.h"

using namespace std;

class LRU
{
private:
	int size;
	int *times;
public:
	LRU(int);
	~LRU() {
		size = 0;
		if (times)
			delete[] times;
	}
	int smallestValue();
	void print();
	void setValue(int value, int position) {
		if (position >= 0 && position < size)
			times[position] = value;
	}
	int getValue(int position) {
		return times[position];
	}
};


class SecondChance
{
private:
	struct Pair
	{
		string page;
		bool referenceBit;
		Pair(string& p) : page(p) { referenceBit = false; }
		void markFalse() { referenceBit = false; }
		void markTrue() { referenceBit = true; }
	};
	deque<Pair> fifo;
public:
	void hit(Memory&, Statistics&, string&);
	int pageFault(Memory&, Statistics&, string&);
	void print();
};

void print(LRU* lru, SecondChance *sc);

#endif
