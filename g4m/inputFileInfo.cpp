#include "inputFileInfo.h"

inputFileInfoT::inputFileInfoT(std::string infoFile, std::string & workingDir)
{
	xmlParser pathsParser(infoFile);
	string pathStr;
	xmlParser::executionResultT execResult;

	// TODO: setting up input file names and (maybe) other parameters passed from GUI
	getValueFromXml("input_dir", folders.inputDir);
	getValueFromXml("output_dir", folders.outputDir);

	// File names
	getValueFromXml("simu_bin_file", files.simuBinFileName);
	getValueFromXml("out_map_name", files.outMapFileName);
	getValueFromXml("out_table_name", files.outTableFileName);

	produceMaps = true;
	produceTables = true;
}

inputFileInfoT::~inputFileInfoT()
{

}