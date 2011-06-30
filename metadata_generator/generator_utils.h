#ifndef GENERATOR_UTILS_H_
#define GENERATOR_UTILS_H_

#include <string>
#include <vector>

using namespace std;

struct constInfoT
{
	string name;
	string value;

	constInfoT(string name, string value): name(name), value(value) {}
};

struct varInfoT
{
	string name;
	string type;
	bool isArray;
	string size;

	varInfoT(string name, string type, bool isArray = false, string size = "")
		: name(name)
		, type(type)
		, isArray(isArray)
		, size(size)
	{}
};

//struct arrayInfoT
//{
//	string name;
//	string type;
//	string size;
//
//	arrayInfoT(string name, string type, string size): name(name), type(type), size(size) {}
//};

struct structInfoT
{
	string name;
	bool isTemplate;
	string templateTypeName;
	vector<varInfoT> fields;
	
	structInfoT(string name, vector<varInfoT> &fields, bool isTemplate, string templateTypeName = "")
		: name(name)
		, fields(fields)
		, isTemplate(isTemplate)
		, templateTypeName(templateTypeName)
	{}
};

bool ParseConst(const vector<string> &words, int &currPos, vector<constInfoT> &constData);
bool ParseVar(const vector<string> &words, int &currPos, vector<varInfoT> &varData);
bool ParseArray(const vector<string> &words, int &currPos, vector<varInfoT> &arrayData);
bool ParseStruct(const vector<string> &words, int &currPos, vector<structInfoT> &structData);
bool ParseStructTemplate(const vector<string> &words, int &currPos, vector<structInfoT> &structData);


#endif