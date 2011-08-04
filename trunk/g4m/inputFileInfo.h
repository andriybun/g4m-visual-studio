#ifndef INPUT_FILE_INFO_H_
#define INPUT_FILE_INFO_H_

#include <string>
#include <assert.h>

#include "xmlParser.h"

#define getValueOrInvalidate(NAME, DEST_VAR)				\
	execResult = pathsParser.getValue(NAME, DEST_VAR);		\
	assert(execResult == xmlParser::EXECUTION_RESULT_OK);


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