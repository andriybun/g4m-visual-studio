#ifndef XML_DATA_H_
#define XML_DATA_H_

#include <iostream>
#include <string> 
#include <vector>
#include <map>

#include <assert.h>

using namespace std;

class xmlData
{
public:
	typedef map<string, string> paramsMapT;
private:
	struct node
	{
		string name;
		paramsMapT params;
		node * upAddr;
		size_t upIdx;
		size_t currIdx;
		vector<node *> downAddr;
		node(): name(""), upAddr(NULL), upIdx(-1), currIdx(-1) {};
	};
	node * currPos;
	node * top;

	void deleteBranch(node * branchTop);
	void printMap(const map<string, string> & stringMap);
	void printBranch(node * branchTop, size_t depth);
public:
	xmlData(void);
	~xmlData(void);

	string getName(node * nodePtr = NULL);

	// add new branch at the same level as currPos
	void addBranch(const string & name);
	void addBranch(const string & name, const paramsMapT & paramsMap);
	// add new node below currPos
	void branchPush(const string & name);
	void branchPush(const string & name, const paramsMapT & paramsMap);

	// navigate over the tree
	node * navigateUp();
	node * navigateNext();
	node * navigatePrev();
	node * navigateDown(size_t idx = 0);

	void print();
};

#endif