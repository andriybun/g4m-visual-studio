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
#include "generate_operators.h"
#include "generate_list_of_fields.h"

using namespace std;

#define MAX_LINE_LENGTH		256

string tempFileName = "no_comments.h";
string autogeneratedMetadata = "autogenerated_metadata.h";
string autogeneratedOperators = "autogenerated_operators.h";
string autogeneratedListFileName = "listOfFields.xml";

int GenerateMetadata(string fileName, string autogeneratedName, string autogeneratedOperatorsName);

int main( int argc, char* argv[] )
{
	if (argc < 2)
	{
		printf("Error! File name not specified.\n");
		return 1;
	}

	string fileName = argv[1];

	int returnValue = GenerateMetadata(fileName, autogeneratedMetadata, autogeneratedOperators);
	if (returnValue)
	{
		printf("Error #%d! File not generated.\n", returnValue);
	}

	printf("Metadata generated successfully\n");
	remove(tempFileName.c_str());
	return returnValue;

}

int GenerateMetadata(string fileName, string autogeneratedName, string autogeneratedOperatorsName)
{
	int removeCommentsRes = RemoveComments(fileName, tempFileName);
	if (removeCommentsRes)
	{
		return removeCommentsRes;
	}

	ifstream file;
	file.open(tempFileName.c_str(), ios::in);

	vector<string> templateTypes;
	templateTypes.push_back("double");
	templateTypes.push_back("float");

	if (file.is_open())
	{
		// storage of generated data and types
		vector<constInfoT> constData;
		vector<varInfoT> arrayData;
		vector<structInfoT> structData;
		vector<string> words;

		// Read file
		string line;
		while(!file.eof())
		{
			vector<string> tokens;
			getline(file, line);
			if (line[0] == '#') continue;
			SurroundMarksWithSpaces(line);
			Tokenize(line, tokens, " \n\t");
			words.insert(words.end(), tokens.begin(), tokens.end());			
		}

		// Parse file
		for (int i = 0; i < words.size(); i++)
		{
			// Constant definition
			if (words[i] == "CONSTDEF")
			{
				if (!ParseConst(words, i, constData)) return 3; // invalid syntax
				continue;
			}

			// Array definition
			if (words[i] == "array")
			{
				if (!ParseArray(words, i, arrayData)) return 3; // invalid syntax
				continue;
			}

			// Struct definition
			if (words[i] == "STRUCTDEF")
			{
				if (!ParseStruct(words, i, structData)) return 3; // invalid syntax
				continue;
			}

			// Template struct definition
			if (words[i] == "template")
			{
				if (!ParseStructTemplate(words, i, structData)) return 3; // invalid syntax
				continue;
			}
		}

		// Write autogenerated interface
		ofstream af;
		af.open(autogeneratedName.c_str(), ios::out);
		af << "#ifndef AUTOGENERATED_METADATA_H_\n";
		af << "#define AUTOGENERATED_METADATA_H_\n";
		af << "// This is autogenerated file.\n";
		af << "// Do not edit it directly.\n\n";
		af << "#include \"struct_info.h\"\n\n";
		af << "#include \"intf.h\"\n\n";
		
		// Write constants
		for (int idx = 0; idx < constData.size(); idx++)
		{
			char line2file[MAX_LINE_LENGTH];
			int lineLen = sprintf(line2file, "//const int %s = %s;\n", constData[idx].name.c_str(), constData[idx].value.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
		}
		af.write("\n", 1);

		// Write arrays
		for (int idx = 0; idx < arrayData.size(); idx++)
		{
			char line2file[MAX_LINE_LENGTH];
			int lineLen = sprintf(line2file, "//%s %s[%s];\n", arrayData[idx].type.c_str(), arrayData[idx].name.c_str(), arrayData[idx].size.c_str());
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
			lineLen = sprintf(line2file, "template <>\n");
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			lineLen = sprintf(line2file, "void struct_info_t< %s >::define_metadata()\n", structData[idx].name.c_str());
			assert(lineLen < MAX_LINE_LENGTH);
			af.write(line2file, lineLen);
			af << "{\n";
			// Write struct fields
			for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
			{
				lineLen = sprintf(line2file, "\tadd_variable(m_struct.%s);\t\t// %s\n", structData[idx].fields[fieldIdx].name.c_str(),
					structData[idx].fields[fieldIdx].type.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
			}
			af << "}\n";
			af.write("\n", 1);
		}

		// Write template structs
		for (int typeIdx = 0; typeIdx < templateTypes.size(); typeIdx++)
		{
			for (int idx = 0; idx < structData.size(); idx++)
			{
				if (!structData[idx].isTemplate) continue;
				char line2file[MAX_LINE_LENGTH];
				int lineLen;
				string structTemplateName = structData[idx].name + "<" + templateTypes[typeIdx] + ">";
				lineLen = sprintf(line2file, "template <>\n");
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				lineLen = sprintf(line2file, "void struct_info_t< %s >::define_metadata()\n", structTemplateName.c_str());
				assert(lineLen < MAX_LINE_LENGTH);
				af.write(line2file, lineLen);
				af << "{\n";
				// Write struct fields
				for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
				{
					string outType = structData[idx].fields[fieldIdx].type;
					FindAndReplace(outType, structData[idx].templateTypeName, templateTypes[typeIdx]);
					lineLen = sprintf(line2file, "\tadd_variable(m_struct.%s);\t\t// %s\n", structData[idx].fields[fieldIdx].name.c_str(), outType.c_str());
					assert(lineLen < MAX_LINE_LENGTH);
					af.write(line2file, lineLen);
				}
				af << "}\n";
				af.write("\n", 1);
			}
		}

		af << "#endif\n";
		af.close();

		GenerateOperators(autogeneratedOperatorsName, structData);
		GenerateListOfFields(autogeneratedListFileName, structData);

	}
	else
	{
		printf("Error! Unable to open file: %s\n", fileName.c_str());
		return 2;
	}

	file.close();
	return 0;
}