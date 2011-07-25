#ifndef INPUT_FILE_INFO_H_
#define INPUT_FILE_INFO_H_

#include <string>

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

	inputFileInfoT(std::string infoFile);
	~inputFileInfoT();
};

#endif