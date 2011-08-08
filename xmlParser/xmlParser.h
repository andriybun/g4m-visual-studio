#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

#include "xmlData.h"

using namespace std;

class xmlParser
{
public:
	enum executionResultT
	{
		EXECUTION_RESULT_OK,
		EXECUTION_RESULT_VALUE_NOT_FOUND
	};
private:
	bool isInitialized;
	vector<string> fileContents;
	vector<bool> fileContentsIsTag;
	size_t currIdx;
	vector<string> tagStack;
	xmlData xmlTree;

	void parseStringToParams(const string & inFullTag, string & tagName, map<string, string> & paramsMap);
	size_t getTag(size_t startIdx);

public:
	//enum executionResultT
	//{
	//	EXECUTION_RESULT_OK,
	//	EXECUTION_RESULT_VALUE_NOT_FOUND
	//};

	xmlParser(void);
	xmlParser(string fileName);
	~xmlParser(void);

	bool initialize(string fileName);
	executionResultT getValue(const string & tagName, string & val);
	executionResultT getValue(const string & tagName, string & val, map<string, string> & paramsMap);
	void parseXmlTree();
	void printXmlTree();
};

#endif