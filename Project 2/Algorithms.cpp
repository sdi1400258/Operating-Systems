#include "Algorithms.h"

LRU::LRU(int s) : size(s)
{
	int i;
	times = new int[s];
	for (i = 0; i < s; ++i)
		times[i] = -1;
}

/*
Prints status for LRU times
 */
void LRU::print()
{
	cout << "LRU times:" << endl;
	int i;
	int s = size;
	for (i = 0; i < s; ++i)
	{
		if (times[i] >= 0)
			cout << i << ": " << times[i];
		else
			cout << i << ": empty";
		if (i == smallestValue())
			cout << "\t<-- oldest";
		cout << endl;
	}
}

/*
Finds the smallest time-value in the LRU table
*/
int LRU::smallestValue()
{
	int old_pos, old_val;

	/* Init values */
	old_pos = -2; //position with smallest value
	old_val = 2000000; //value of above position
	old_val++;

	for (int i = 0; i < size; )
	{
		if (times[i] <= -1) //empty position
		{
			old_pos = i;
			return i;
		}
		if (times[i] < old_val) //smaller value found
		{
			old_val = times[i];
			old_pos = i;
		}
		i++;
	}

	return old_pos;
}

void SecondChance::hit(Memory& memory, Statistics& stats, string& newPage)
{
	size_t i = 0;
	while (i != fifo.size())
	{
		if (strcmp(fifo[i].page.c_str(), newPage.c_str()) == 0)
		{
			fifo[i].markTrue();
			break;
		}
		i++;
	}
}

int SecondChance::pageFault(Memory& memory, Statistics& stats, string& newPage)
{
	int pos;
	string pageToRemove;
	pos = 0;

	do
	{
		if (!fifo[0].referenceBit || (int)fifo.size() < memory.getSize())
		{
			if ((int)fifo.size() == memory.getSize())
			{
				pageToRemove.clear();
				pageToRemove = fifo[0].page;
				pos = memory.searchPageInMemory(pageToRemove);
				memory.replacePage(newPage, pos, &stats);
				fifo.pop_front();
			}
			else
				pos = memory.addPage(newPage, &stats);
			fifo.push_back(Pair(newPage));
			break;
		}
		else
		{
			fifo.front().markFalse();
			fifo.push_back(fifo[0]);
			fifo.pop_front();
		}
	} while(true);
	return pos;
}

void SecondChance::print()
{
	cout << "Second Chance queue:" << endl;
	size_t i = 0;
	cout << "old" << endl;
	while (i != fifo.size())
	{
		cout << i << ": [";
		cout << fifo[i].page.data() << " " << fifo[i].referenceBit << ']' << endl;
		i++;
	}
	cout << "new" << endl;
}

void print(LRU* lru, SecondChance *sc)
{
	if (lru)
		lru->print();
	else if (sc)
		sc->print();
}
