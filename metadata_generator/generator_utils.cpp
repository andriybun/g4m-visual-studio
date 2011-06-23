#include "generator_utils.h"

bool ParseConst(const vector<string> &words, int &currPos, vector<constInfoT> &constData)
{
	if (words[++currPos] != "(") return false; // invalid syntax
	string constName = words[++currPos];
	if (words[++currPos] != ",") return false; // invalid syntax
	string constValue = words[++currPos];
	while (words[++currPos] != ")")
	{
		constValue += words[currPos];
	}
	constData.push_back(constInfoT(constName, constValue));
	return true;
}

bool ParseVar(const vector<string> &words, int &currPos, vector<varInfoT> &varData)
{
	string varType = words[++currPos];
	string varName = words[++currPos];
	if (words[++currPos] != ";") return false; // invalid syntax
	varData.push_back(varInfoT(varName, varType));
	return true;
}

bool ParseArray(const vector<string> &words, int &currPos, vector<arrayInfoT> &arrayData)
{
	if (words[++currPos] != "(") return false; // invalid syntax
	string typeName = words[++currPos];
	if (words[++currPos] != ",") return false; // invalid syntax
	string arrayName = words[++currPos];
	if (words[++currPos] != ",") return false; // invalid syntax
	string arraySize = words[++currPos];
	if (words[++currPos] != ")") return false; // invalid syntax
	if (words[++currPos] != ";") return false; // invalid syntax
	arrayData.push_back(arrayInfoT(arrayName, typeName, arraySize));
	return true;
}

bool ParseStruct(const vector<string> &words, int &currPos, vector<structInfoT> &structData)
{
	vector<varInfoT> varData;

	if (words[++currPos] != "(") return false; // invalid syntax
	string structName = words[++currPos];
	if (words[++currPos] != ")") return false; // invalid syntax
	if (words[++currPos] != "{") return false; // invalid syntax
	while (words[++currPos] != "}")
	{
		if (words[currPos] == "array")
		{
			vector<arrayInfoT> dummy;
			if (!ParseArray(words, currPos, dummy)) return false; // invalid syntax
			varData.push_back(varInfoT(dummy[0].name, dummy[0].type + "[" + dummy[0].size + "]"));
		}
		else
		{
			if (!ParseVar(words, --currPos, varData)) return false; // invalid syntax
		}
	}
	structData.push_back(structInfoT(structName, varData, false));
	return true;
}

bool ParseStructTemplate(const vector<string> &words, int &currPos, vector<structInfoT> &structData)
{
	vector<varInfoT> varData;

	if (words[++currPos] != "<") return false; // invalid syntax
	while (words[++currPos] != ">") {}
	string templateTypeName = words[currPos-1];

	if (words[++currPos] != "STRUCTDEF") return false; // invalid syntax
	if (words[++currPos] != "(") return false; // invalid syntax
	string structName = words[++currPos];
	if (words[++currPos] != ")") return false; // invalid syntax
	if (words[++currPos] != "{") return false; // invalid syntax
	while (words[++currPos] != "}")
	{
		if (words[currPos] == "array")
		{
			vector<arrayInfoT> dummy;
			if (!ParseArray(words, currPos, dummy)) return false; // invalid syntax
			varData.push_back(varInfoT(dummy[0].name, dummy[0].type + "[" + dummy[0].size + "]"));
		}
		else
		{
			if (!ParseVar(words, --currPos, varData)) return false; // invalid syntax
		}
	}
	structData.push_back(structInfoT(structName, varData, true, templateTypeName));

	return true;
}