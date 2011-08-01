#ifndef INPUT_FILE_INFO_H_
#define INPUT_FILE_INFO_H_

#include <string>
#include <assert.h>

#include "xmlParser.h"

#define getValueFromXml(NAME, PATH)						\
	execResult = pathsParser.getValue(NAME, pathStr);	\
	assert(execResult == EXECUTION_RESULT_OK);			\
	PATH = pathStr;


// These structures should contain all file names used within the model

struct dirInfoT
{
	std::string inputDir;
	std::string outputDir;
};

struct fileInfoT
{
	std::string simuBinFileName;
	std::string cellDataTxt;

	std::string outMapFileName;
	std::string outTableFileName;
};

class inputFileInfoT
{
private:
	
public:
	dirInfoT folders;
	fileInfoT files;

	bool produceMaps;
	bool produceTables;

	inputFileInfoT(std::string infoFile, std::string & workingDir);
	~inputFileInfoT();
};

#endif