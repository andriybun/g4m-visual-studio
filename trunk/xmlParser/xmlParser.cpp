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
		return true;
	}
	else
	{
		printf("Error! Can not open file.\n");
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
			parseStringToParams(fileContents[idx], paramsMap);
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

void xmlParser::parseStringToParams(string & tag, map<string, string> & paramsMap)
{
	vector<string> tokens;
	SurroundMarksWithSpaces("=", tag);
	Tokenize(tag, tokens, " ");
	size_t idx = 1;
	while (idx < tokens.size())
	{
		assert(idx + 3 <= tokens.size());
		assert(tokens[idx+1] == "=");
		paramsMap.insert(pair<string, string>(tokens[idx], tokens[idx+2]));
		idx += 3;
	}
}

xmlParser::executionResultT xmlParser::parseContentsToSubtags(size_t startFromIdx, const string & tagName, vector<string> & valueVector)
{
	//size_t tagNameSize = tagName.size();
	//for (size_t idx = 0; idx < fileContents.size(); idx++)
	//{
	//	if (fileContentsIsTag[idx] && (fileContents[idx].substr(0, tagNameSize) == tagName))
	//	{
	//		parseStringToParams(fileContents[idx], paramsMap);
	//		idx++;
	//		while(fileContents[idx] != "/" + tagName)
	//		{
	//			val += fileContents[idx];
	//			idx++;
	//		}
	//		return EXECUTION_RESULT_OK;
	//	}
	//}

	return EXECUTION_RESULT_OK;
}