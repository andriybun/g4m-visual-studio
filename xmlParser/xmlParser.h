#ifndef XML_PARSER_H_
#define XML_PARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <assert.h>

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

	void parseStringToParams(string & tag, map<string, string> & paramsMap);
	executionResultT parseContentsToSubtags(size_t startFromIdx, const string & tagName, vector<string> & valueVector);
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
};

#endif