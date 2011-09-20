#include "xmlParser.h"
#include "string_utils.h"

xmlParser::xmlParser(void)
{
	isInitialized = false;
	currIdx = 0;
}

xmlParser::xmlParser(string fileName)
{
	isInitialized = false;
	currIdx = 0;
	isInitialized = initialize(fileName);
}

xmlParser::~xmlParser(void)
{
}

bool xmlParser::initialize(string fileName)
{
	assert(!isInitialized);
	ifstream f;
	f.open(fileName.c_str(), ios::in);
	if (f.is_open())
	{
		string line;
		while(!f.eof())
		{
			getline(f, line);
			if (line.size() == 0 || (line[0] == '#')) continue;
			//SurroundMarksWithSpaces("<>=", line);
			vector<string> tokens;
			vector<isTagT> tokensIsTag;
			Tokenize(line, tokens, tokensIsTag, "<>\t");
			for (int idx = 0; idx < tokens.size(); idx++)
			{
				fileContents.push_back(tokens[idx]);
				fileContentsIsTag.push_back(tokensIsTag[idx] == IS_TAG);
			}
		}
		f.close();
		parseXmlTree();
		return true;
	}
	else
	{
		printf("Error! Can not open xml file: %s\n", fileName.c_str());
		assert(false);
		return false;
	}
}

xmlParser::executionResultT xmlParser::getValue(const string & tagName, string & val)
{
	val = "";
	size_t tagNameSize = tagName.size();
	for (size_t idx = 0; idx < fileContents.size(); idx++)
	{
		if (fileContentsIsTag[idx] && (fileContents[idx].substr(0, tagNameSize) == tagName))
		{
			idx++;
			while(fileContents[idx] != "/" + tagName)
			{
				val += fileContents[idx];
				idx++;
			}
			return EXECUTION_RESULT_OK;
		}
	}
	return EXECUTION_RESULT_VALUE_NOT_FOUND;
}

xmlParser::executionResultT xmlParser::getValue(const string & tagName, string & val, map<string, string> & paramsMap)
{
	val = "";
	size_t tagNameSize = tagName.size();
	for (size_t idx = 0; idx < fileContents.size(); idx++)
	{
		if (fileContentsIsTag[idx] && (fileContents[idx].substr(0, tagNameSize) == tagName))
		{
			string tagName;
			parseStringToParams(fileContents[idx], tagName, paramsMap);
			idx++;
			while(fileContents[idx] != "/" + tagName)
			{
				val += fileContents[idx];
				idx++;
			}
			return EXECUTION_RESULT_OK;
		}
	}
	return EXECUTION_RESULT_VALUE_NOT_FOUND;
}

xmlParser::executionResultT xmlParser::getSubtags(const string & tagName,
												  vector<string> & result,
												  vector<xmlData::paramsMapT> & resultParams,
												  const pair<string, string> & paramsMask,
												  const string & skipTag)
{
	xmlTree.getSubtags(tagName, result, resultParams, paramsMask, skipTag);
	return EXECUTION_RESULT_OK;
}

xmlParser::executionResultT xmlParser::getSubtags(const string & tagName,
												  vector<int> & result,
												  vector<xmlData::paramsMapT> & resultParams,
												  const pair<string, string> & paramsMask,
												  const string & skipTag)
{
	vector<string> resultStr;
	getSubtags(tagName, resultStr, resultParams, paramsMask, skipTag);
	for (size_t idx = 0; idx < resultStr.size(); idx++)
	{
		result.push_back(atoi(resultStr[idx].c_str()));
	}
	return EXECUTION_RESULT_OK;
}


void xmlParser::parseStringToParams(const string & inFullTag, string & tagName, map<string, string> & paramsMap)
{
	vector<string> tokens;
	string tmpTag = inFullTag;
	SurroundMarksWithSpaces("=", tmpTag);
	Tokenize(tmpTag, tokens, " ");
	tagName = tokens[0];
	size_t idx = 1;
	while (idx < tokens.size())
	{
		assert(idx + 3 <= tokens.size());
		assert(tokens[idx+1] == "=");
		FindAndReplaceAll(tokens[idx+2], "\"", "");
		paramsMap.insert(pair<string, string>(tokens[idx], tokens[idx+2]));
		idx += 3;
	}
}

size_t xmlParser::getTag(size_t startIdx)
{
	string inFullTag = fileContents[startIdx];
	string tagName;
	map<string, string> params;
	if (fileContentsIsTag[startIdx])
	{
		parseStringToParams(inFullTag, tagName, params);
		xmlTree.branchPush(tagName, params);
		tagStack.push_back("/" + tagName);
		while (!(fileContentsIsTag[++startIdx] && (fileContents[startIdx] == tagStack[tagStack.size()-1])))
		{
			startIdx = getTag(startIdx);
		}
		//if (params.size() > 0)
		//{
		//	xmlTree.navigateUp();
		//}
		xmlTree.navigateUp();
		tagStack.pop_back();
	}
	else
	{
		while (!fileContentsIsTag[startIdx])
		{
			xmlTree.branchPush(fileContents[startIdx++]);
			xmlTree.navigateUp();
		}
		startIdx--;
	}
	return startIdx;
}

void xmlParser::parseXmlTree()
{
	size_t idx = 0;
	while (idx < fileContents.size())
	{
		idx = getTag(idx);
		idx++;		
	}
}

void xmlParser::printXmlTree()
{
	xmlTree.print();
}