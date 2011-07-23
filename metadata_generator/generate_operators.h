#include <iostream>
#include <fstream>

#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>

#include <assert.h>

#include "string_utils.h"
#include "generator_utils.h"

#define MAX_LINE_LENGTH		256

static vector<string> binaryOperatorsList;
static vector<string> binaryFunctionsList;

void WriteBinaryOperatorStructStruct(ofstream & af, const structInfoT & structInfo);
void WriteBinaryOperatorStructVar(ofstream & af, const structInfoT & structInfo);
void WriteBinaryFunctionsStructStruct(ofstream & af, const structInfoT & structInfo);
void WriteBinaryFunctionsStructVar(ofstream & af, const structInfoT & structInfo);

void GenerateOperators(const string & autogeneratedOperatorsName, const vector<structInfoT> & structData)
{
	// operators list
	binaryOperatorsList.push_back("+");
	binaryOperatorsList.push_back("-");
	binaryOperatorsList.push_back("*");
	binaryOperatorsList.push_back("/");
	// functions list
	binaryFunctionsList.push_back("min");
	binaryFunctionsList.push_back("max");

	ofstream af;
	af.open(autogeneratedOperatorsName.c_str(), ios::out);
	af << "#ifndef AUTOGENERATED_OPERATORS_H_\n";
	af << "#define AUTOGENERATED_OPERATORS_H_\n";
	af << "// This is autogenerated file.\n";
	af << "// Do not edit it directly.\n\n";
	af << "#include \"intf.h\"\n\n";
	af << "#define xxx_min(x, y) ((x)<(y) ? (x):(y))\n";
	af << "#define xxx_max(x, y) ((x)<(y) ? (y):(x))\n\n";

	//af << "#include \"operatorHelpers.h\"\n\n";

	// Write structs
	for (int idx = 0; idx < structData.size(); idx++)
	{
		WriteBinaryOperatorStructStruct(af, structData[idx]);
		WriteBinaryOperatorStructVar(af, structData[idx]);
		WriteBinaryFunctionsStructStruct(af, structData[idx]);
		WriteBinaryFunctionsStructVar(af, structData[idx]);
	}

	af << "#include \"../commonClasses/struct_info.h\"\n\n";
	af << "#endif\n";
	af.close();

}

void WriteBinaryOperatorStructStruct(ofstream & af, const structInfoT & structInfo)
{
	char line2file[MAX_LINE_LENGTH];
	int lineLen;
	string structName;
	string templatePrefix = "";
	if (structInfo.isTemplate)
	{
		templatePrefix += "template <class " + structInfo.templateTypeName + ">\n";
		structName = structInfo.name + "<" + structInfo.templateTypeName + ">";
	}
	else
	{
		structName = structInfo.name;
	}
	vector<string>::iterator it = binaryOperatorsList.begin();
	while(it != binaryOperatorsList.end())
	{
		af << templatePrefix;
		lineLen = sprintf(line2file, "%s operator %s (%s & first, %s & second)\n", 
			structName.c_str(),
			(*it).c_str(),
			structName.c_str(), 
			structName.c_str());
		assert(lineLen < MAX_LINE_LENGTH);
		af.write(line2file, lineLen);
		af << "{\n";
		af << "\t" << structName.c_str() << " result;\n";
		for (int fieldIdx = 0; fieldIdx < structInfo.fields.size(); fieldIdx++)
		{
			if (!structInfo.fields[fieldIdx].isArray)
			{
				lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second.%s;\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
			}
			else
			{
				af << "\tfor (int g00 = 0; g00 < " << structInfo.fields[fieldIdx].size << "; g00++)\n";
				af << "\t{\n";
				lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second.%s[g00];\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				af << "\t}\n";
			}
		}
		af << "\treturn result;\n";
		af << "}\n\n";
		it++;
	}
}

void WriteBinaryOperatorStructVar(ofstream & af, const structInfoT & structInfo)
{
	char line2file[MAX_LINE_LENGTH];
	int lineLen;
	string structName;
	string templatePrefix = "";
	if (structInfo.isTemplate)
	{
		templatePrefix += "template <class " + structInfo.templateTypeName + ", typename variableXxxType>\n";
		structName = structInfo.name + "<" + structInfo.templateTypeName + ">";
	}
	else
	{
		templatePrefix += "template <typename variableXxxType>\n";
		structName = structInfo.name;
	}
	vector<string>::iterator it = binaryOperatorsList.begin();
	while(it != binaryOperatorsList.end())
	{
		af << templatePrefix;
		lineLen = sprintf(line2file, "%s operator %s (%s & first, variableXxxType & second)\n", 
			structName.c_str(),
			(*it).c_str(),
			structName.c_str());
		assert(lineLen < MAX_LINE_LENGTH);
		af.write(line2file, lineLen);
		af << "{\n";
		af << "\t" << structName.c_str() << " result;\n";
		for (int fieldIdx = 0; fieldIdx < structInfo.fields.size(); fieldIdx++)
		{
			if (!structInfo.fields[fieldIdx].isArray)
			{
				lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second;\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
			}
			else
			{
				af << "\tfor (int g00 = 0; g00 < " << structInfo.fields[fieldIdx].size << "; g00++)\n";
				af << "\t{\n";
				lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second;\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				af << "\t}\n";
			}
		}
		af << "\treturn result;\n";
		af << "}\n\n";
		it++;
	}
}

void WriteBinaryFunctionsStructStruct(ofstream & af, const structInfoT & structInfo)
{
	char line2file[MAX_LINE_LENGTH];
	int lineLen;
	string structName;
	string templatePrefix = "";
	if (structInfo.isTemplate)
	{
		templatePrefix += "template <class " + structInfo.templateTypeName + ">\n";
		structName = structInfo.name + "<" + structInfo.templateTypeName + ">";
	}
	else
	{
		structName = structInfo.name;
	}
	vector<string>::iterator it = binaryFunctionsList.begin();
	while(it != binaryFunctionsList.end())
	{
		af << templatePrefix;
		lineLen = sprintf(line2file, "%s %s(%s & first, %s & second)\n", 
			structName.c_str(),
			(*it).c_str(),
			structName.c_str(), 
			structName.c_str());
		assert(lineLen < MAX_LINE_LENGTH);
		af.write(line2file, lineLen);
		af << "{\n";
		af << "\t" << structName.c_str() << " result;\n";
		for (int fieldIdx = 0; fieldIdx < structInfo.fields.size(); fieldIdx++)
		{
			if (!structInfo.fields[fieldIdx].isArray)
			{
				lineLen = sprintf(line2file, "\tresult.%s = xxx_%s(first.%s, second.%s);\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
			}
			else
			{
				af << "\tfor (int g00 = 0; g00 < " << structInfo.fields[fieldIdx].size << "; g00++)\n";
				af << "\t{\n";
				lineLen = sprintf(line2file, "\t\tresult.%s[g00] = xxx_%s(first.%s[g00], second.%s[g00]);\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				af << "\t}\n";
			}
		}
		af << "\treturn result;\n";
		af << "}\n\n";
		it++;
	}
}

void WriteBinaryFunctionsStructVar(ofstream & af, const structInfoT & structInfo)
{
	char line2file[MAX_LINE_LENGTH];
	int lineLen;
	string structName;
	string templatePrefix = "";
	if (structInfo.isTemplate)
	{
		templatePrefix += "template <class " + structInfo.templateTypeName + ", typename variableXxxType>\n";
		structName = structInfo.name + "<" + structInfo.templateTypeName + ">";
	}
	else
	{
		templatePrefix += "template <typename variableXxxType>\n";
		structName = structInfo.name;
	}
	vector<string>::iterator it = binaryFunctionsList.begin();
	while(it != binaryFunctionsList.end())
	{
		af << templatePrefix;
		lineLen = sprintf(line2file, "%s %s(%s & first, variableXxxType & second)\n", 
			structName.c_str(),
			(*it).c_str(),
			structName.c_str(), 
			structName.c_str());
		assert(lineLen < MAX_LINE_LENGTH);
		af.write(line2file, lineLen);
		af << "{\n";
		af << "\t" << structName.c_str() << " result;\n";
		for (int fieldIdx = 0; fieldIdx < structInfo.fields.size(); fieldIdx++)
		{
			if (!structInfo.fields[fieldIdx].isArray)
			{
				lineLen = sprintf(line2file, "\tresult.%s = xxx_%s(first.%s, second);\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
			}
			else
			{
				af << "\tfor (int g00 = 0; g00 < " << structInfo.fields[fieldIdx].size << "; g00++)\n";
				af << "\t{\n";
				lineLen = sprintf(line2file, "\t\tresult.%s[g00] = xxx_%s(first.%s[g00], second);\n", 
					structInfo.fields[fieldIdx].name.c_str(),
					(*it).c_str(),
					structInfo.fields[fieldIdx].name.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				af << "\t}\n";
			}
		}
		af << "\treturn result;\n";
		af << "}\n\n";
		it++;
	}
}