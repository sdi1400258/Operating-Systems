#include <cstring>
#include "Memory.h"

static list<Frame*>::iterator it; //iterator for the list

Memory::Memory(int memorySize) : memSize(memorySize)
{
	hashTable = new list<Frame*>[memorySize];
	frames = new Frame*[memorySize];
	for (int i = 0; i < memorySize; )
	{
		frames[i] = new Frame(i);
		i++;
	}
}

Memory::~Memory()
{
	for (int i = 0; i < memSize; )
	{
		if (frames[i])
			delete frames[i];
		i++;
	}
	if (frames)
		delete[] frames;
	if (hashTable)
		delete[] hashTable;
}

void Memory::markDirty(int position)
{
	frames[position]->markDirty();
}

int Memory::searchPageInMemory(string page)
{
	int i = hashFunction(page);
	if (i < 0)
		return -1;
	for (it = hashTable[i].begin(); it != hashTable[i].end(); it++)
		if (!strcmp((*it)->getPage().c_str(), page.c_str()))
			return (*it)->getPos();
	return -1;
}

int Memory::hashFunction(string page)
{
	unsigned int total = 0;
	for (size_t i = 0; i < strlen(page.c_str()); )
	{
		total += page.c_str()[i];
		i++;
	}
	return total % memSize;
}

int Memory::addPage(string page, Statistics* stats)
{
	int i;
	for (i = 0; i < memSize; i++)
		if (frames[i]->getPage().length() == 0)
		{
			//set the 'page' at the new 'position'
			frames[i]->setPage(page);

			//inc reads counter
			stats->readsInc();

			//update hash table entry
			int newPos = hashFunction(page);
			hashTable[newPos].push_back(frames[i]);
			break;
		}
	return i;
}

void Memory::replacePage(string page, int position, Statistics* stats)
{
	int i;
	string pageToBeDeleted = frames[position]->getPage();
	i = 0;
	i = hashFunction(pageToBeDeleted);

	//Remove the current page from 'position'
	bool found = false;
	for (it = hashTable[i].begin(); it != hashTable[i].end() && found == false; it++)
	{
		if (!(*it)->getPage().compare(pageToBeDeleted))
		{
			hashTable[i].erase(it);
			found = true;
		}
	}

	//if dirty --> writes++
	if (frames[position]->isDirty())
		stats->writesInc();

	//inc reads counter
	stats->readsInc();

	//set the 'page' at the new 'position'
	frames[position]->setPage(page);

	//update hash table entry
	hashTable[hashFunction(page)].push_back(frames[position]);
}

void Memory::print()
{
	cout << "Memory:" << endl;
	int i;
	i = 0;
	while (i < memSize)
	{
		cout << i << ":\t";
		string dirty = frames[i]->isDirty() ? "dirty" : " ";
		cout << frames[i]->getPage() << " ";
		cout << dirty << endl;
		i++;
	}

	cout << endl;
	cout << "HashTable: " << endl;
	i = 0;
	while (i < memSize)
	{
		cout << i << ":\t";
		it = hashTable[i].begin();
		while (it != hashTable[i].end())
		{
			cout << "(" << (*it)->getPage() << ",";
			cout << (*it)->getPos() << ") ";
			it++;
		}
		cout << endl;
		i++;
	}
}
