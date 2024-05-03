#ifndef TRACE_H_
#define TRACE_H_

#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

/*
Class for each trace (read from input files)
*/
class Trace
{
	string page;
	char pageType;
public:
	Trace(string p)
	{
		pageType = ' ';
		page = p.substr(0, 5); //first 5 chars of line
		pageType = *p.substr(9, 1).c_str(); //9th char of line
	}
	void clear() { page.clear(); }
	char getType() const { return pageType; }
	string getPage() { return page; }
};

#endif
