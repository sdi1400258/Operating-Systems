#include <iostream>
#include <fstream>
#include <cstdlib>
#include "Memory.h"
#include "Algorithms.h"
#include "Statistics.h"
#include "InputInfo.h"
#include "Trace.h"

using namespace std;

int main(int argc, char* argv[])
{
	unsigned int time, source=0;
	int max = 2000000;
	LRU *lru = NULL;
	SecondChance* sc = NULL;

	if (argc != 7 && argc != 6) //check arguments
	{
		cout << "Wrong arguments. Correct usage is:" << endl;
		cout << argv[0] << " input_file1 input_file2 [lru | sc] q memory_size [maximum]" << endl;
		return 0;
	}

	if (argc == 7)
		max = atoi(argv[6]); //set max to maximum available value
	Statistics* stats = new Statistics();

	if (max >= 2000001 || max <= 0)
	{
		cout << "Maximum value should be less or equal to 2000000." << endl;
		return 0;
	}

	int pos = 0;
	InputInfo input(string(argv[3]), atoi(argv[4]), atoi(argv[5]), max, argv[1], argv[2]);

	//check algorithm input
	if (input.getAlgorithm() != "lru" && input.getAlgorithm() != "sc")
	{
		cout << "Algorithm argument should be either lru or sc" << endl;
		exit(0);
	}

	Memory mem(input.getMemorySize());
	if (input.getAlgorithm() == "lru")
		lru = new LRU(atoi(argv[5]));
	else
		sc = new SecondChance();

	for (time = 1; time <= input.getMaximum(); time++)
	{
		input.selectIn(source);
		if (input.checkEOFs())
			break;

		string line;
		getline(*input.getIn(), line); //read line from current input

		if (line.length() < 10)
			continue;
		Trace trace(line); //create trace from line

		if ((int)time < 2 * input.getQ() || max - (int)time < input.getQ())
			cout << "Time: " << time << ", read: " << trace.getPage() << endl;

		if (input.getAlgorithm() == "lru") //LRU
		{
			pos = mem.searchPageInMemory(trace.getPage());
			if (pos < 0) //page fault
			{
				stats->pageFaultsInc(); //new page fault
				pos = lru->smallestValue(); //find LRU frame
				mem.replacePage(trace.getPage(), pos, stats); //replace that position
			}
			else
				stats->hitsInc(); //hit

			if (trace.getType() != 'R') //if page is changed
				mem.markDirty(pos); //mark it as dirty
			lru->setValue(time, pos); //update LRU times
		}
		else //Second Chance
		{
			pos = mem.searchPageInMemory(trace.getPage());
			if (pos >= 0) //hit
			{
				stats->hitsInc(); //hit
				string newPage = trace.getPage();
				sc->hit(mem, *stats, newPage);
			}
			else
			{
				stats->pageFaultsInc(); //new page fault
				string newPage = trace.getPage();
				pos = sc->pageFault(mem, *stats, newPage);
			}

			if (trace.getType() == 'W') //if page is changed
				mem.markDirty(pos); //mark it as dirty
		}

		if ((int)time < 2 * input.getQ() || max - (int)time < input.getQ())
		{
			mem.print();
			cout << endl;
			print(lru, sc);
			cout << "---------------" << endl << endl;
		}

		//Change source if q traces read
		if (time % input.getQ() == 0)
		{
			source = source + 1;
			source %= 2;
		}
	}

	//Print statistics
	stats->printStatistics(input.getAlgorithm().c_str(), input.getQ(), input.getMemorySize(), max);

	delete stats;
	//Free memory for algorithm
	if (lru)
		delete lru;
	else
		delete sc;
	return 0;
}
