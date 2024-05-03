#ifndef STATISTICS_H
#define STATISTICS_H

#include <iostream>
#include <string>

using namespace std;

/*
Class for required statistics: reads from disk, writes to disk, 
page faults and hits.
*/

class Statistics
{
	int hits, pageFaults, reads, writes;
	void printInput(const char* alg, int q, int memSize, int max) const
	{
		cout << "algorithm used:\t" << alg << endl;
		cout << "memory size:\t" << memSize << endl;
		cout << "value of q:\t" << q << endl;
		cout << "max traces:\t" << max << endl << endl;
	}
	void printHits() const
	{
		cout << "Hits / PageFaults:\t";
		cout << hits << " / " << pageFaults << endl;
	}
	void printWR() const
	{
		cout << "Reads / Writes:  ";
		cout << "\t";
		cout << reads;
		cout << " / ";
		cout << writes << endl;
	}
public:
	Statistics() { pageFaults = 0; hits = 0; writes = 0; reads = 0; }
	void writesInc() { writes+=1; }
	void readsInc() { reads+=1; }
	void pageFaultsInc() { pageFaults+=1; }
	void hitsInc() { hits+=1; }

	void printStatistics(const char* alg, int q, int memSize, int max) const
	{
		cout << "End of simulation." << endl << "Input:" << endl;
		printInput(alg, q, memSize, max);
		cout << "Results:" << endl;
		printWR();
		printHits();
	}
};

#endif
