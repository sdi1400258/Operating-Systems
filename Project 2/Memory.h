#ifndef MEMORY_H
#define MEMORY_H

#include <iostream>
#include <string>
#include <list>
#include "Statistics.h"

using namespace std;

/*
Frame in memory
*/
class Frame
{
	int position; //page's position in memory
	string page; //page
	bool dirty; //page is dirty or not
public:
	Frame(int pos, bool dirt = false)
		{ dirty = dirt; position = pos;}
	void markDirty() { dirty = true; }
	bool isDirty() const { return dirty; }
	void initDirty() { dirty = false; }
	void setPage(string p) { page = p; dirty = false; }
	int getPos() const { return position; }
	string getPage() { return page; }
};

/*
Represents the main memory
*/
class Memory
{
	list<Frame*> *hashTable; //hash tabl
	int memSize; //count of memory frames
	Frame **frames; //memory frames
	int hashFunction(string);
public:
	Memory(int);
	~Memory();
	int searchPageInMemory(string page);
	int getSize() const { return memSize; }
	void markDirty(int position);
	int addPage(string, Statistics*);
	void replacePage(string page, int position, Statistics*);
	Frame* getFrame(int i) { return frames[i]; }
	void print();
};

#endif
