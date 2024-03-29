#ifndef GENERATE_LIST_OF_FIELDS_H_
#define GENERATE_LIST_OF_FIELDS_H_

#include <iostream>
#include <fstream>
#include <string>

#include <vector>

#define MAX_LINE_LENGTH		256

void GenerateListOfFields(const string & autogeneratedListFileName, const vector<structInfoT> & structData)
{
	ofstream af;
	af.open(autogeneratedListFileName.c_str(), ios::out);

	for (int idx = 0; idx < structData.size(); idx++)
	{
		//std::cout << structData[idx].name << "\n";
		af << "<struct name =\""<< structData[idx].name << "\">\n";
		for (int fieldIdx = 0; fieldIdx < structData[idx].fields.size(); fieldIdx++)
		{
			//std::cout << "\t" << structData[idx].fields[fieldIdx].name << "\n";
			af << "\t<field>" << structData[idx].fields[fieldIdx].name << "</field>\n";
		}
		af << "</struct>\n";
	}

	af.close();
}

#endif