#ifndef INPUTINFO_H_
#define INPUTINFO_H_

#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>

using namespace std;

/*
Class to keep all input information, which is not related to a
replacement algorithm.
*/
class InputInfo
{
	ifstream in1, in2, *in;
	string algorithm;
	unsigned q, memorySize, maximum;
public:
	InputInfo(string alg, int Q, int memSize, int maxTraces, char* filename1, char* filename2) : q(Q)
	{
		algorithm = alg;
		maximum = maxTraces;
		memorySize = memSize;
		in1.open(filename1);
		if (!in1.good()) //check if files opened
		{
			cerr << "Error while opening input file: " << string(filename1) << endl;
			exit(1);
		}
		in = &in1;
		in2.open(filename2);
		if (!in2.good()) //check if files opened
		{
			cerr << "Error while opening input file: " << string(filename2) << endl;
			exit(1);
		}
	}
	~InputInfo()
	{
		in2.close();
		in1.close();
	}
	int getMemorySize() const { return memorySize; }
	unsigned getMaximum() const { return maximum; }
	string getAlgorithm() const { return algorithm; }
	int getQ() const { return q; }
	ifstream* getIn() { return in; }
	void selectIn(int source)
	{
		if (source == 0)
			in = &in1;
		else
			in = &in2;
	}
	bool firstInput()
	{
		if (in == &in1)
			return true;
		return false;
	}
	bool checkEOFs()
	{
		if (in1.eof() && in2.eof())
			return true;
		return false;
	}
};

#endif
