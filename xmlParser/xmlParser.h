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
		EXECUTION_RESULT_VALUE_NOT_FOUND,
		EXECUTION_RESULT_INVALID_TYPE
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
	executionResultT getSubtags(const string & tagName,
		vector<string> & result,
		vector<xmlData::paramsMapT> & resultParams, 
		const pair<string, string> & paramsMask = pair<string, string>("", ""),
		const string & skipTag = "");
	executionResultT getSubtags(const string & tagName,
		vector<int> & result,
		vector<xmlData::paramsMapT> & resultParams, 
		const pair<string, string> & paramsMask = pair<string, string>("", ""),
		const string & skipTag = "");
	void parseXmlTree();
	void printXmlTree();
};

#endif