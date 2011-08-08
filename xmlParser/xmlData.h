#ifndef XML_DATA_H_
#define XML_DATA_H_

#include <iostream>
#include <string> 
#include <vector>

#include <assert.h>

using namespace std;

class xmlData
{
private:
	struct node
	{
		string name;
		node * upAddr;
		size_t upIdx;
		size_t currIdx;
		vector<node *> downAddr;
		node(): name(""), upAddr(NULL), upIdx(-1), currIdx(-1) {};
	};
	node * currPos;
	node * top;

	void deleteBranch(node * branchTop);
	void printBranch(node * branchTop, size_t depth);
public:
	xmlData(void);
	~xmlData(void);

	string getName(node * nodePtr = NULL);

	// add new branch at the same level as currPos
	void addBranch(string name);
	// add new node below currPos
	void branchPush(string name);

	// navigate over the tree
	node * navigateUp();
	node * navigateNext();
	node * navigatePrev();
	node * navigateDown(size_t idx = 0);

	void print();
};

#endif