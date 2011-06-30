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

void GenerateOperators(const string autogeneratedOperatorsName, const vector<constInfoT> &constData, const vector<structInfoT> &structData)
{
	vector<string> operatorsList;
	operatorsList.push_back("+");
	operatorsList.push_back("-");
	operatorsList.push_back("*");
	operatorsList.push_back("/");

	ofstream af;
	af.open(autogeneratedOperatorsName.c_str(), ios::out);
	af << "#ifndef AUTOGENERATED_OPERATORS_H_\n";
	af << "#define AUTOGENERATED_OPERATORS_H_\n";
	af << "// This is autogenerated file.\n";
	af << "// Do not edit it directly.\n\n";
	af << "#include \"intf.h\"\n\n";
	//af << "#include \"operatorHelpers.h\"\n\n";

	// Write constants
	for (int idx = 0; idx < constData.size(); idx++)
	{
		char line2file[MAX_LINE_LENGTH];
		int lineLen = sprintf(line2file, "const int %s = %s;\n", constData[idx].name.c_str(), constData[idx].value.c_str());
		assert(lineLen < MAX_LINE_LENGTH);
		af.write(line2file, lineLen);
	}
	af.write("\n", 1);

	// Write structs
	for (int idx = 0; idx < structData.size(); idx++)
	{
		if (structData[idx].isTemplate) continue;
		char line2file[MAX_LINE_LENGTH];
		int lineLen;
		vector<string>::iterator it = operatorsList.begin();
		while(it != operatorsList.end())
		{
			// operators: "{struct} {operator} {struct}"
			lineLen = sprintf(line2file, "%s operator %s (%s & first, %s & second)\n", 
				structData[idx].name.c_str(),
				(*it).c_str(),
				structData[idx].name.c_str(), 
				structData[idx].name.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			af << "{\n";
			af << "\t" << structData[idx].name.c_str() << " result;\n";
			for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
			{
				if (!structData[idx].fields[fieldIdx].isArray)
				{
					lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second.%s;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str(),
						structData[idx].fields[fieldIdx].name.c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
				}
				else
				{
					af << "\tfor (int g00 = 0; g00 < " << structData[idx].fields[fieldIdx].size << "; g00++)\n";
					af << "\t{\n";
					lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second.%s[g00];\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str(),
						structData[idx].fields[fieldIdx].name.c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
					af << "\t}\n";
				}
			}
			af << "\treturn result;\n";
			af << "}\n\n";
			// operators: "{struct} {operator} {variable}"
			af << "template <typename variableXxxType>\n";
			lineLen = sprintf(line2file, "%s operator %s (%s & first, variableXxxType & second)\n", 
				structData[idx].name.c_str(),
				(*it).c_str(),
				structData[idx].name.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			af << "{\n";
			af << "\t" << structData[idx].name.c_str() << " result;\n";
			for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
			{
				if (!structData[idx].fields[fieldIdx].isArray)
				{
					lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
				}
				else
				{
					af << "\tfor (int g00 = 0; g00 < " << structData[idx].fields[fieldIdx].size << "; g00++)\n";
					af << "\t{\n";
					lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
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

	// Write template structs
	for (int idx = 0; idx < structData.size(); idx++)
	{
		if (!structData[idx].isTemplate) continue;
		char line2file[MAX_LINE_LENGTH];
		int lineLen;
		string templateTypeName = structData[idx].templateTypeName;
		string structName = structData[idx].name;
		vector<string>::iterator it = operatorsList.begin();
		while(it != operatorsList.end())
		{
			// operators: "{struct} {operator} {struct}"
			lineLen = sprintf(line2file, "template <class %s>\n", templateTypeName.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			lineLen = sprintf(line2file, "%s<%s> operator %s (%s<%s> & first, %s<%s> & second)\n", 
				structData[idx].name.c_str(),
				templateTypeName.c_str(),
				(*it).c_str(),
				structData[idx].name.c_str(), 
				templateTypeName.c_str(),
				structData[idx].name.c_str(),
				templateTypeName.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			af << "{\n";
			af << "\t" << structData[idx].name << "<" << templateTypeName << "> result;\n";
			for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
			{
				if (!structData[idx].fields[fieldIdx].isArray)
				{
					lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second.%s;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str(),
						structData[idx].fields[fieldIdx].name.c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
				}
				else
				{
					af << "\tfor (int g00 = 0; g00 < " << structData[idx].fields[fieldIdx].size << "; g00++)\n";
					af << "\t{\n";
					lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second.%s[g00];\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str(),
						structData[idx].fields[fieldIdx].name.c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
					af << "\t}\n";
				}
			}
			af << "\treturn result;\n";
			af << "}\n\n";
			// operators: "{struct} {operator} {variable}"
			lineLen = sprintf(line2file, "template <class %s, typename variableXxxType>\n", templateTypeName.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			lineLen = sprintf(line2file, "%s<%s> operator %s (%s<%s> & first, variableXxxType & second)\n", 
				structData[idx].name.c_str(),
				templateTypeName.c_str(),
				(*it).c_str(),
				structData[idx].name.c_str(), 
				templateTypeName.c_str(),
				structData[idx].name.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			af << "{\n";
			af << "\t" << structData[idx].name << "<" << templateTypeName << "> result;\n";
			for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
			{
				if (!structData[idx].fields[fieldIdx].isArray)
				{
					lineLen = sprintf(line2file, "\tresult.%s = first.%s %s second;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
						(*it).c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
				}
				else
				{
					af << "\tfor (int g00 = 0; g00 < " << structData[idx].fields[fieldIdx].size << "; g00++)\n";
					af << "\t{\n";
					lineLen = sprintf(line2file, "\t\tresult.%s[g00] = first.%s[g00] %s second;\n", 
						structData[idx].fields[fieldIdx].name.c_str(),
						structData[idx].fields[fieldIdx].name.c_str(),
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

	af << "#endif\n";
	af.close();

}