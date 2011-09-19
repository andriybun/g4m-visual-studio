#include "xmlData.h"

xmlData::xmlData(void)
{
	top = new node;
	top->name = "root";
	currPos = top;
}

xmlData::~xmlData(void)
{
	deleteBranch(top);
}

void xmlData::addBranch(const string & name)
{
	assert(currPos->upAddr != NULL);
	node * newBranchTop = new node;
	newBranchTop->name = name;
	newBranchTop->upAddr = currPos->upAddr;
	newBranchTop->upIdx = currPos->upIdx;
	newBranchTop->currIdx = currPos->currIdx + 1;
	currPos->upAddr->downAddr.push_back(newBranchTop);
	currPos = newBranchTop;
}

void xmlData::addBranch(const string & name, const paramsMapT & paramsMap)
{
	assert(currPos->upAddr != NULL);
	node * newBranchTop = new node;
	newBranchTop->name = name;
	newBranchTop->params = paramsMap;
	newBranchTop->upAddr = currPos->upAddr;
	newBranchTop->upIdx = currPos->upIdx;
	newBranchTop->currIdx = currPos->currIdx + 1;
	currPos->upAddr->downAddr.push_back(newBranchTop);
	currPos = newBranchTop;
}

void xmlData::branchPush(const string & name)
{
	node * newNode = new node;
	newNode->name = name;
	newNode->upAddr = currPos;
	newNode->upIdx = currPos->downAddr.size();
	newNode->currIdx = 0;
	currPos->downAddr.push_back(newNode);
	currPos = newNode;
}

void xmlData::branchPush(const string & name, const paramsMapT & paramsMap)
{
	node * newNode = new node;
	newNode->name = name;
	newNode->params = paramsMap;
	newNode->upAddr = currPos;
	newNode->upIdx = currPos->downAddr.size();
	newNode->currIdx = 0;
	currPos->downAddr.push_back(newNode);
	currPos = newNode;
}

string xmlData::getName(xmlData::node * nodePtr)
{
	if (nodePtr == NULL)
	{
		return currPos->name;
	}
	else
	{
		nodePtr->name;
	}
}

xmlData::node * xmlData::navigateUp()
{
	if (currPos->upAddr == NULL)
	{
		// TODO: maybe assert here?
		return NULL;
	}
	else
	{
		return currPos = currPos->upAddr;
	}
	
}

xmlData::node * xmlData::navigateNext()
{
	size_t newIdx = currPos->upIdx+1;
	if (newIdx >= currPos->upAddr->downAddr.size())
	{
		// TODO: maybe assert here?
		return NULL;
	}
	else
	{
		return currPos = currPos->upAddr->downAddr[newIdx];
	}
}

xmlData::node * xmlData::navigatePrev()
{
	if (currPos->upIdx == 0)
	{
		// TODO: maybe assert here?
		return NULL;
	}
	else
	{
		size_t newIdx = currPos->upIdx-1;
		return currPos = currPos->upAddr->downAddr[newIdx];
	}
}

xmlData::node * xmlData::navigateDown(size_t idx)
{
	if ((currPos->downAddr.size() == 0) || (idx >= currPos->downAddr.size()-1))
	{
		// TODO: maybe assert here?
		return NULL;
	}
	else
	{
		return currPos = currPos->downAddr[idx];
	}

}

void xmlData::deleteBranch(node * branchTop)
{
	while (branchTop->downAddr.size() > 0)
	{
		node * nodeToDelete = branchTop->downAddr[branchTop->downAddr.size()-1];
		deleteBranch(nodeToDelete);
		branchTop->downAddr.pop_back();
	}
	delete branchTop;
}

void xmlData::print()
{
	printBranch(top, 0);	
}

void xmlData::printBranch(node * branchTop, size_t depth)
{
	for (size_t tb = 0; tb < depth; tb++) printf("\t");
	printf("%s ", branchTop->name.c_str());
	printMap(branchTop->params);
	printf("\n");
	for (size_t idx = 0; idx < branchTop->downAddr.size(); idx++)
	{
		node * nodeToPrint = branchTop->downAddr[idx];
		printBranch(nodeToPrint, depth+1);
	}
}

void xmlData::printMap(const map<string, string> & stringMap)
{
	printf("(");
	map<string, string>::const_iterator it = stringMap.begin();
	while (it != stringMap.end())
	{
		if (it != stringMap.begin())
		{
			printf("; ");
		}
		printf("%s: %s", (*it).first.c_str(), (*it).second.c_str());
		it++;
	}
	printf(")");
}